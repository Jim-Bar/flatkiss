#include "Position.hpp"

Position::Position(size_t X, size_t Y) : X(X), Y(Y) {

}

size_t Position::x() const {
    return X;
}

size_t Position::y() const {
    return Y;
}
