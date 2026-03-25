#include <pybind11/pybind11.h>
#include <cmath>
#include <algorithm>

float get_angle(float x1, float y1, float x2, float y2) {
    // 向量內積 (Dot product)
    float dot = x1 * x2 + y1 * y2;
    // 向量長度 (Magnitude)
    float mag1 = std::sqrt(x1 * x1 + y1 * y1);
    float mag2 = std::sqrt(x2 * x2 + y2 * y2);

    // 老師在 validate.sh 中要求的：處理無效輸入 (Zero-length vector)
    // 這裡我們預期它回傳 0.0 或拋出異常。
    if (mag1 == 0 || mag2 == 0) {
        return 0.0; 
    }

    float cos_theta = dot / (mag1 * mag2);

    // 數值穩定性：防止浮點數誤差導致 cos 略大於 1 或小於 -1
    // 例如 1.0000001 會導致 acos 報錯，我們強制限縮在 [-1, 1]
    if (cos_theta > 1.0f) cos_theta = 1.0f;
    if (cos_theta < -1.0f) cos_theta = -1.0f;

    return std::acos(cos_theta);
}

PYBIND11_MODULE(_vector, m) {
    m.def("angle", &get_angle, "Calculate the angle between two 2D vectors");
}