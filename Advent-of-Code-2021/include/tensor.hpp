#ifndef _TENSOR_H_
#define _TENSOR_H_

#include <vector>

typedef unsigned uint;

namespace tensor {

    template <typename T>
    class Tensor {
        std::vector<T> _data;
        uint _xdim, _ydim, _zdim;
    public:
        Tensor(uint xdim, uint ydim, uint zdim) : _xdim(xdim), _ydim(ydim), _zdim(zdim) {
            _data.resize(xdim*ydim*zdim);
        }

        void fill(T value) {
            std::fill(_data.begin(), _data.end(), value);
        }

        size_t size() const {
            return _data.size();
        }

        void shape(uint* shapes) const {
            shapes[0] = _xdim;
            shapes[1] = _ydim;
            shapes[2] = _zdim;
        }

        T& operator()(uint x, uint y, uint z) {
            if (x >= _xdim || y >= _ydim || z >= _zdim)
                throw std::out_of_range("Indices out of range");
            return _data[_xdim * _ydim * z + _xdim * y + x];
        }

        T operator()(uint x, uint y, uint z) const {
            if (x >= _xdim || y >= _ydim || z >= _zdim)
                throw std::out_of_range("Indices out of range");
            return _data[_xdim * _ydim * z + _xdim * y + x];
        }

        T& operator()(uint index) {
            if (index >= _data.size())
                throw std::out_of_range("Indices out of range");
            return _data[index];
        }

        T sum_single_z_dim(uint z) {
            if (z >= _zdim)
                throw std::out_of_range("Index out of range");
            T sum = 0;
            for (int i = _xdim * _ydim * z; i < _xdim * _ydim * (z + 1); i++) {
                if (_data[i] != -1) sum += _data[i];
            }
            return sum;
        }

        template<typename U>
        friend std::ostream& operator<<(std::ostream& stream, const Tensor<U>& object) {
            stream << '[';
            for (int i = 0; i < object._zdim; i++) {
                if (i > 0) stream << ' ';
                stream << '[';
                for (int j = 0; j < object._xdim; j++) {
                    if (j > 0) stream << "  ";
                    stream << '[';
                    for (int k = 0; k < object._ydim; k++) {
                        stream << object(j, k, i);
                        if (k < object._ydim - 1) stream << ' ';
                    }
                    stream << ']';
                    if (j < object._xdim - 1) stream << std::endl;
                }
                stream << ']';
                if (i < object._zdim - 1) stream << std::endl;
            }
            stream << "] Tensor [" << object._xdim << ',' << object._ydim << ',' << object._zdim << ']';
            return stream;
        }
    };

}

#endif