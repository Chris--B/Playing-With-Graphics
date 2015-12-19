#pragma once

template <typename T, typename U>
T as(U thing) {
    return static_cast<T>(thing);
}

float getRand(float min, float max) {
    float unit = as<float>(rand()) / RAND_MAX;
    return (max - min) * unit + min;
}
