function read_data(path::AbstractString)
    lines = open(path, "r") do io
        readlines(io)
    end
    char_map = Dict{Char, Int}('=' => -2, '-' => -1, '0' => 0, '1' => 1, '2' => 2)
    fuels = [[char_map[c] for c in collect(line)] for line in lines]
    return fuels
end

function decode(code::Vector{Int})
    result = 0
    for i in length(code)-1:-1:0
        result += code[length(code) - i] * (5 ^ i)
    end
    return result
end

function encode(value::Int)
    encoded = Vector{Int}()
    while true
        remainder = value % 5
        pushfirst!(encoded, remainder)
        if value < 5
            pushfirst!(encoded, 0)
            break
        end
        value = value ÷ 5
    end
    for i in length(encoded):-1:1
        if encoded[i] >= 3
            encoded[i] -= 5
            encoded[i - 1] += 1
        end
    end
    char_map = Dict{Int, Char}(-2 => '=', -1 => '-', 0 => '0', 1 => '1', 2 => '2')
    encoded = string([char_map[x] for x in encoded]...)
    return encoded
end

function main()
    fuels = read_data("data/25_data.txt")
    fuel_sum = sum([decode(f) for f in fuels])
    @show encode(fuel_sum)
end

if abspath(PROGRAM_FILE) == @__FILE__
    main()
end
