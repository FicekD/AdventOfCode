@enum Resource begin
    ore = 1
    clay = 2
    obsidian = 3
    geode = 4
end

struct Recipe
    resource::Resource
    count::Int
end

mutable struct Strategy
    robots::Dict{Resource, Int}
    resources::Dict{Resource, Int}
    skipped_resources::Set{Resource}
    score::Int
end

function read_data(path::AbstractString)
    lines = open(path, "r") do io
        readlines(io)
    end
    blueprints = Vector{Dict{Resource, Vector{Recipe}}}()
    for line in lines
        m = match(r"Blueprint \d+: Each ore robot costs (\d+) ore. Each clay robot costs (\d+) ore. Each obsidian robot costs (\d+) ore and (\d+) clay. Each geode robot costs (\d+) ore and (\d+) obsidian.", line)
        m = [parse(Int, m_i) for m_i in collect(m)]
        blueprint = Dict{Resource, Vector{Recipe}}(
            ore => [Recipe(ore, m[1])],
            clay => [Recipe(ore, m[2])],
            obsidian => [Recipe(ore, m[3]), Recipe(clay, m[4])],
            geode => [Recipe(ore, m[5]), Recipe(obsidian, m[6])]
        )
        push!(blueprints, blueprint)
    end
    return blueprints
end

function simulate_robots(blueprint, strategies, limit)
    resources = [ore, clay, obsidian]
    max_costs = Dict{Resource, Int}(
        ore => maximum([blueprint[r][1].count for r in [ore, clay, obsidian, geode]]),
        clay => blueprint[obsidian][2].count,
        obsidian => blueprint[geode][2].count
    )
    branch_strat = x -> Strategy(copy(x.robots), copy(x.resources), Set{Resource}(), x.score)
    for i in range(1, limit)
        time_left = limit - i
        branched_staregies = Strategy[]
        for strat in strategies
            if all([strat.resources[r.resource] >= r.count for r in blueprint[geode]])
                for r in blueprint[geode] 
                    strat.resources[r.resource] -= r.count
                end
                strat.score += time_left
            else
                if (strat.resources[ore] >= blueprint[ore][1].count) && strat.robots[ore] < max_costs[ore] && !(ore in strat.skipped_resources)
                    branched_strat = branch_strat(strat)
                    branched_strat.robots[ore] += 1
                    branched_strat.resources[ore] -= blueprint[ore][1].count + 1
                    push!(branched_staregies, branched_strat)
                    push!(strat.skipped_resources, ore)
                end
                if strat.resources[ore] >= blueprint[clay][1].count && strat.robots[clay] < max_costs[clay] && !(clay in strat.skipped_resources)
                    branched_strat = branch_strat(strat)
                    branched_strat.robots[clay] += 1
                    branched_strat.resources[ore] -= blueprint[clay][1].count
                    branched_strat.resources[clay] -= 1
                    push!(branched_staregies, branched_strat)
                    push!(strat.skipped_resources, clay)
                end
                if all([strat.resources[r.resource] >= r.count for r in blueprint[obsidian]]) && strat.robots[obsidian] < max_costs[obsidian] && !(obsidian in strat.skipped_resources)
                    branched_strat = branch_strat(strat)
                    branched_strat.robots[obsidian] += 1
                    for r in blueprint[obsidian]
                        branched_strat.resources[r.resource] -= r.count
                    end
                    branched_strat.resources[obsidian] -= 1
                    push!(branched_staregies, branched_strat)
                    push!(strat.skipped_resources, obsidian)
                end
            end
        end
        strategies = cat(strategies, branched_staregies, dims=1)
        for strat in strategies
            for r in resources
                strat.resources[r] += strat.robots[r]
            end
        end
    end
    return strategies
end

function main()
    blueprints = read_data("data/19_data.txt")

    qualities = Int[]
    for (i, blueprint) in enumerate(blueprints)
        strategies = [Strategy(Dict{Resource, Int}(ore => 1, clay => 0, obsidian => 0),
                               Dict{Resource, Int}(ore => 0, clay => 0, obsidian => 0), Set{Resource}(), 0)]
        strategies = simulate_robots(blueprint, strategies, 24)
        push!(qualities, i * maximum([strat.score for strat in strategies]))
    end
    @show sum(qualities)

    scores = Int[]
    for blueprint in blueprints[1:(length(blueprints) > 2 ? 3 : 2)]
        strategies = [Strategy(Dict{Resource, Int}(ore => 1, clay => 0, obsidian => 0),
                               Dict{Resource, Int}(ore => 0, clay => 0, obsidian => 0), Set{Resource}(), 0)]
        strategies = simulate_robots(blueprint, strategies, 32)
        push!(scores, maximum([strat.score for strat in strategies]))
    end
    @show prod(scores)
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
