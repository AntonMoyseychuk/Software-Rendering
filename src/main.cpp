#include "math/lin_math.hpp"
#include "math/mat3x3.hpp"
#include "math/vector.hpp"

#include <iostream>
#include <algorithm>
#include <vector>


static const std::uint16_t w = 640, h = 480;

#define TO_RASTER(screen_w, screen_h, v) \
    math::Vector3((screen_w) * (1.0f + v.X()) / 2, (screen_h) * (1.0f + v.Y()) / 2, 1.0f)

void OutputFrame(const std::vector<math::Vector3>& frameBuffer, const char* filename) {
    FILE* pFile = nullptr;
    fopen_s(&pFile, filename, "w");
    fprintf(pFile, "P3\n%d %d\n%d\n ", w, h, 255);
    for (auto i = 0; i < w * h; ++i) {
        uint32_t r = static_cast<uint32_t>(255 * std::clamp(frameBuffer[i].X(), 0.0f, 1.0f));
        uint32_t g = static_cast<uint32_t>(255 * std::clamp(frameBuffer[i].Y(), 0.0f, 1.0f));
        uint32_t b = static_cast<uint32_t>(255 * std::clamp(frameBuffer[i].Z(), 0.0f, 1.0f));
        fprintf(pFile, "%d %d %d ", r, g, b);
    }
    fclose(pFile);
}


int main(int argc, char* argv[]) {
    using namespace math;

    Vector3 v0(-0.5,  0.5, 1.0);
    Vector3 v1( 0.5,  0.5, 1.0);
    Vector3 v2( 0.0, -0.5, 1.0);

    v0 = TO_RASTER(w, h, v0);
    v1 = TO_RASTER(w, h, v1);
    v2 = TO_RASTER(w, h, v2);

    Vector3 c0(1, 0, 0);
    Vector3 c1(0, 1, 0);
    Vector3 c2(0, 0, 1);

    auto M = LinMath::Transpose(Mat3x3(v0, v1, v2));

    try {
        M = LinMath::Inverse(M);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return -1;
    }

    math::Vector3 E0 = math::Vector3(1, 0, 0) * M;
    math::Vector3 E1 = math::Vector3(0, 1, 0) * M;
    math::Vector3 E2 = math::Vector3(0, 0, 1) * M;

    std::vector<math::Vector3> frameBuffer(w * h);

    for (auto y = 0; y < h; y++) {
        for (auto x = 0; x < w; x++) {
            math::Vector3 sample = { x + 0.5f, y + 0.5f, 1.0f };

            float alpha = LinMath::Dot(E0, sample);
            float beta = LinMath::Dot(E1, sample);
            float gamma = LinMath::Dot(E2, sample);

            if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f) {
                //#define BLENDING
                #ifdef BLENDING
                    frameBuffer[x + y * w] = Vector3(c0 * alpha + c1 * beta + c2 * gamma);
                #else
                    frameBuffer[x + y * w] = Vector3(1, 0, 1);
                #endif
            }
        }
    }

    OutputFrame(frameBuffer, "D:\\Studies\\Graphics\\software_rendering\\out\\render_hello_triangle.ppm");

    return 0;
}