#include "mat4.hpp"

#include "vec3.hpp"
#include "vec_operations.hpp"

#include "util.hpp"

#include <cassert>

namespace math {
    const mat4f mat4f::IDENTITY(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    mat4f::mat4f() noexcept
        : mm_row0(_mm_setr_ps(1.0f, 0.0f, 0.0f, 0.0f)), 
            mm_row1(_mm_setr_ps(0.0f, 1.0f, 0.0f, 0.0f)),
            mm_row2(_mm_setr_ps(0.0f, 0.0f, 1.0f, 0.0f)), 
            mm_row3(_mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f))
    {
    }

    mat4f::mat4f(
        float v00, float v01, float v02, float v03,
        float v04, float v05, float v06, float v07,
        float v08, float v09, float v10, float v11,
        float v12, float v13, float v14, float v15) noexcept
        : vec_row0(v00, v01, v02, v03), vec_row1(v04, v05, v06, v07), vec_row2(v08, v09, v10, v11), vec_row3(v12, v13, v14, v15)
    {}
    
    mat4f::mat4f(const vec4f &vec_row0, const vec4f &vec_row1, const vec4f &vec_row2, const vec4f &vec_row3) noexcept
        : vec_row0(vec_row0), vec_row1(vec_row1), vec_row2(vec_row2), vec_row3(vec_row3)
    {
    }
    
    mat4f::mat4f(const __m128 &row0, const __m128 &row1, const __m128 &row2, const __m128 &row3) noexcept
        : mm_row0(row0), mm_row1(row1), mm_row2(row2), mm_row3(row3)
    {
    }

    mat4f::mat4f(const mat4f &mat) noexcept
        : mm_row0(mat.mm_row0), mm_row1(mat.mm_row1), mm_row2(mat.mm_row2), mm_row3(mat.mm_row3)
    {
    }

    mat4f &mat4f::operator=(const mat4f &mat) noexcept {
        mm_row0 = mat.mm_row0; 
        mm_row1 = mat.mm_row1; 
        mm_row2 = mat.mm_row2; 
        mm_row3 = mat.mm_row3;

        return *this;
    }

    mat4f mat4f::operator-() const noexcept {
        return mat4f(-vec_row0, -vec_row1, -vec_row2, -vec_row3);
    }

    mat4f mat4f::operator+(const mat4f &mat) const noexcept {
        return mat4f(
            _mm_add_ps(mm_row0, mat.mm_row0),
            _mm_add_ps(mm_row1, mat.mm_row1),
            _mm_add_ps(mm_row2, mat.mm_row2),
            _mm_add_ps(mm_row3, mat.mm_row3)
        );
    }

    mat4f &mat4f::operator+=(const mat4f &mat) noexcept {
        mm_row0 = _mm_add_ps(mm_row0, mat.mm_row0);
        mm_row1 = _mm_add_ps(mm_row1, mat.mm_row1);
        mm_row2 = _mm_add_ps(mm_row2, mat.mm_row2);
        mm_row3 = _mm_add_ps(mm_row3, mat.mm_row3);

        return *this;
    }

    mat4f mat4f::operator-(const mat4f &mat) const noexcept {
        return mat4f(
            _mm_sub_ps(mm_row0, mat.mm_row0),
            _mm_sub_ps(mm_row1, mat.mm_row1),
            _mm_sub_ps(mm_row2, mat.mm_row2),
            _mm_sub_ps(mm_row3, mat.mm_row3)
        );
    }

    mat4f &mat4f::operator-=(const mat4f &mat) noexcept {
        mm_row0 = _mm_sub_ps(mm_row0, mat.mm_row0);
        mm_row1 = _mm_sub_ps(mm_row1, mat.mm_row1);
        mm_row2 = _mm_sub_ps(mm_row2, mat.mm_row2);
        mm_row3 = _mm_sub_ps(mm_row3, mat.mm_row3);

        return *this;
    }

    mat4f mat4f::operator*(float value) const noexcept {
        const __m128 mul = _mm_set_ps1(value);
        return mat4f(
            _mm_mul_ps(mm_row0, mul),
            _mm_mul_ps(mm_row1, mul),
            _mm_mul_ps(mm_row2, mul),
            _mm_mul_ps(mm_row3, mul)
        );
    }

    mat4f &mat4f::operator*=(float value) noexcept {
        const __m128 mul = _mm_set_ps1(value);

        mm_row0 = _mm_mul_ps(mm_row0, mul);
        mm_row1 = _mm_mul_ps(mm_row1, mul);
        mm_row2 = _mm_mul_ps(mm_row2, mul);
        mm_row3 = _mm_mul_ps(mm_row3, mul);

        return *this;
    }

    mat4f mat4f::operator/(float value) const noexcept {
        const __m128 div = _mm_set_ps1(value);
        return mat4f(
            _mm_div_ps(mm_row0, div),
            _mm_div_ps(mm_row1, div),
            _mm_div_ps(mm_row2, div),
            _mm_div_ps(mm_row3, div)
        );
    }

    mat4f &mat4f::operator/=(float value) noexcept {
        const __m128 div = _mm_set_ps1(value);

        mm_row0 = _mm_div_ps(mm_row0, div);
        mm_row1 = _mm_div_ps(mm_row1, div);
        mm_row2 = _mm_div_ps(mm_row2, div);
        mm_row3 = _mm_div_ps(mm_row3, div);

        return *this;
    }

    mat4f mat4f::operator*(const mat4f &mat) const noexcept {
        const mat4f transposed = transpose(mat);

        vec4f row0(
            _mm_cvtss_f32(_mm_dp_ps(mm_row0, transposed.mm_row0, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row0, transposed.mm_row1, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row0, transposed.mm_row2, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row0, transposed.mm_row3, 0xF1))
        );

        vec4f row1(
            _mm_cvtss_f32(_mm_dp_ps(mm_row1, transposed.mm_row0, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row1, transposed.mm_row1, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row1, transposed.mm_row2, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row1, transposed.mm_row3, 0xF1))
        );

        vec4f row2(
            _mm_cvtss_f32(_mm_dp_ps(mm_row2, transposed.mm_row0, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row2, transposed.mm_row1, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row2, transposed.mm_row2, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row2, transposed.mm_row3, 0xF1))
        );

        vec4f row3(
            _mm_cvtss_f32(_mm_dp_ps(mm_row3, transposed.mm_row0, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row3, transposed.mm_row1, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row3, transposed.mm_row2, 0xF1)),
            _mm_cvtss_f32(_mm_dp_ps(mm_row3, transposed.mm_row3, 0xF1))
        );

        return mat4f(row0, row1, row2, row3);
    }

    mat4f &mat4f::operator*=(const mat4f &mat) noexcept {
        *this = *this * mat;

        return *this;
    }

    vec4f &mat4f::operator[](size_t i) noexcept {
        assert(i < raw_count);
        return vec_arr[i];
    }

    const vec4f &mat4f::operator[](size_t i) const noexcept {
        assert(i < raw_count);
        return vec_arr[i];
    }

    float mat4f::determinant() const noexcept {
        // _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(add)

        //T SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
        //T SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
        //T SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
        //T SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
        //T SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
        //T SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

        // First 2 columns
        __m128 Swp2A = _mm_shuffle_ps(mm_arr[2], mm_arr[2], _MM_SHUFFLE(0, 1, 1, 2));
        __m128 Swp3A = _mm_shuffle_ps(mm_arr[3], mm_arr[3], _MM_SHUFFLE(3, 2, 3, 3));
        __m128 MulA = _mm_mul_ps(Swp2A, Swp3A);

        // Second 2 columns
        __m128 Swp2B = _mm_shuffle_ps(mm_arr[2], mm_arr[2], _MM_SHUFFLE(3, 2, 3, 3));
        __m128 Swp3B = _mm_shuffle_ps(mm_arr[3], mm_arr[3], _MM_SHUFFLE(0, 1, 1, 2));
        __m128 MulB = _mm_mul_ps(Swp2B, Swp3B);

        // Columns subtraction
        __m128 SubE = _mm_sub_ps(MulA, MulB);

        // Last 2 rows
        __m128 Swp2C = _mm_shuffle_ps(mm_arr[2], mm_arr[2], _MM_SHUFFLE(0, 0, 1, 2));
        __m128 Swp3C = _mm_shuffle_ps(mm_arr[3], mm_arr[3], _MM_SHUFFLE(1, 2, 0, 0));
        __m128 MulC = _mm_mul_ps(Swp2C, Swp3C);
        __m128 SubF = _mm_sub_ps(_mm_movehl_ps(MulC, MulC), MulC);

        //detail::tvec4<T> DetCof(
        //	+ (m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
        //	- (m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
        //	+ (m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
        //	- (m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

        __m128 SubFacA = _mm_shuffle_ps(SubE, SubE, _MM_SHUFFLE(2, 1, 0, 0));
        __m128 SwpFacA = _mm_shuffle_ps(mm_arr[1], mm_arr[1], _MM_SHUFFLE(0, 0, 0, 1));
        __m128 MulFacA = _mm_mul_ps(SwpFacA, SubFacA);

        __m128 SubTmpB = _mm_shuffle_ps(SubE, SubF, _MM_SHUFFLE(0, 0, 3, 1));
        __m128 SubFacB = _mm_shuffle_ps(SubTmpB, SubTmpB, _MM_SHUFFLE(3, 1, 1, 0));//SubF[0], SubE[3], SubE[3], SubE[1];
        __m128 SwpFacB = _mm_shuffle_ps(mm_arr[1], mm_arr[1], _MM_SHUFFLE(1, 1, 2, 2));
        __m128 MulFacB = _mm_mul_ps(SwpFacB, SubFacB);

        __m128 SubRes = _mm_sub_ps(MulFacA, MulFacB);

        __m128 SubTmpC = _mm_shuffle_ps(SubE, SubF, _MM_SHUFFLE(1, 0, 2, 2));
        __m128 SubFacC = _mm_shuffle_ps(SubTmpC, SubTmpC, _MM_SHUFFLE(3, 3, 2, 0));
        __m128 SwpFacC = _mm_shuffle_ps(mm_arr[1], mm_arr[1], _MM_SHUFFLE(2, 3, 3, 3));
        __m128 MulFacC = _mm_mul_ps(SwpFacC, SubFacC);

        __m128 AddRes = _mm_add_ps(SubRes, MulFacC);
        __m128 DetCof = _mm_mul_ps(AddRes, _mm_setr_ps( 1.0f,-1.0f, 1.0f,-1.0f));

        //return m[0][0] * DetCof[0]
        //	 + m[0][1] * DetCof[1]
        //	 + m[0][2] * DetCof[2]
        //	 + m[0][3] * DetCof[3];

        return _mm_cvtss_f32(_mm_dp_ps(mm_arr[0], DetCof, 0xff));
    }

    mat4f operator*(float value, const mat4f& mat) noexcept {
        return mat * value;
    }

    vec4f operator*(const vec3f &vec, const mat4f& mat) noexcept {
        return vec4f(vec, 1.0f) * mat;
    }

    vec4f operator*(const vec4f &vec, const mat4f& mat) noexcept {
        const mat4f transposed = transpose(mat);
        return vec4f(
            dot(vec, transposed.vec_row0), 
            dot(vec, transposed.vec_row1),
            dot(vec, transposed.vec_row2),  
            dot(vec, transposed.vec_row3)  
        );
    }

    mat4f inverse(const mat4f &mat) noexcept {
        assert(is_tends_to(mat.determinant(), 0.0f) == false);

        mat4f out;

        __m128 Fac0;
        {
            //	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
            //	valType SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
            //	valType SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac0 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac1;
        {
            //	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
            //	valType SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
            //	valType SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac1 = _mm_sub_ps(Mul00, Mul01);
        }


        __m128 Fac2;
        {
            //	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
            //	valType SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
            //	valType SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac2 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac3;
        {
            //	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
            //	valType SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
            //	valType SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(3, 3, 3, 3));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(3, 3, 3, 3));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac3 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac4;
        {
            //	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
            //	valType SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
            //	valType SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(2, 2, 2, 2));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(2, 2, 2, 2));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac4 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 Fac5;
        {
            //	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
            //	valType SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
            //	valType SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

            __m128 Swp0a = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(1, 1, 1, 1));
            __m128 Swp0b = _mm_shuffle_ps(mat.mm_arr[3], mat.mm_arr[2], _MM_SHUFFLE(0, 0, 0, 0));

            __m128 Swp00 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(0, 0, 0, 0));
            __m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
            __m128 Swp03 = _mm_shuffle_ps(mat.mm_arr[2], mat.mm_arr[1], _MM_SHUFFLE(1, 1, 1, 1));

            __m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
            __m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
            Fac5 = _mm_sub_ps(Mul00, Mul01);
        }

        __m128 SignA = _mm_set_ps( 1.0f,-1.0f, 1.0f,-1.0f);
        __m128 SignB = _mm_set_ps(-1.0f, 1.0f,-1.0f, 1.0f);

        // m[1][0]
        // m[0][0]
        // m[0][0]
        // m[0][0]
        __m128 Temp0 = _mm_shuffle_ps(mat.mm_arr[1], mat.mm_arr[0], _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Vec0 = _mm_shuffle_ps(Temp0, Temp0, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][1]
        // m[0][1]
        // m[0][1]
        // m[0][1]
        __m128 Temp1 = _mm_shuffle_ps(mat.mm_arr[1], mat.mm_arr[0], _MM_SHUFFLE(1, 1, 1, 1));
        __m128 Vec1 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][2]
        // m[0][2]
        // m[0][2]
        // m[0][2]
        __m128 Temp2 = _mm_shuffle_ps(mat.mm_arr[1], mat.mm_arr[0], _MM_SHUFFLE(2, 2, 2, 2));
        __m128 Vec2 = _mm_shuffle_ps(Temp2, Temp2, _MM_SHUFFLE(2, 2, 2, 0));

        // m[1][3]
        // m[0][3]
        // m[0][3]
        // m[0][3]
        __m128 Temp3 = _mm_shuffle_ps(mat.mm_arr[1], mat.mm_arr[0], _MM_SHUFFLE(3, 3, 3, 3));
        __m128 Vec3 = _mm_shuffle_ps(Temp3, Temp3, _MM_SHUFFLE(2, 2, 2, 0));

        // col0
        // + (Vec1[0] * Fac0[0] - Vec2[0] * Fac1[0] + Vec3[0] * Fac2[0]),
        // - (Vec1[1] * Fac0[1] - Vec2[1] * Fac1[1] + Vec3[1] * Fac2[1]),
        // + (Vec1[2] * Fac0[2] - Vec2[2] * Fac1[2] + Vec3[2] * Fac2[2]),
        // - (Vec1[3] * Fac0[3] - Vec2[3] * Fac1[3] + Vec3[3] * Fac2[3]),
        __m128 Mul00 = _mm_mul_ps(Vec1, Fac0);
        __m128 Mul01 = _mm_mul_ps(Vec2, Fac1);
        __m128 Mul02 = _mm_mul_ps(Vec3, Fac2);
        __m128 Sub00 = _mm_sub_ps(Mul00, Mul01);
        __m128 Add00 = _mm_add_ps(Sub00, Mul02);
        __m128 Inv0 = _mm_mul_ps(SignB, Add00);

        // col1
        // - (Vec0[0] * Fac0[0] - Vec2[0] * Fac3[0] + Vec3[0] * Fac4[0]),
        // + (Vec0[0] * Fac0[1] - Vec2[1] * Fac3[1] + Vec3[1] * Fac4[1]),
        // - (Vec0[0] * Fac0[2] - Vec2[2] * Fac3[2] + Vec3[2] * Fac4[2]),
        // + (Vec0[0] * Fac0[3] - Vec2[3] * Fac3[3] + Vec3[3] * Fac4[3]),
        __m128 Mul03 = _mm_mul_ps(Vec0, Fac0);
        __m128 Mul04 = _mm_mul_ps(Vec2, Fac3);
        __m128 Mul05 = _mm_mul_ps(Vec3, Fac4);
        __m128 Sub01 = _mm_sub_ps(Mul03, Mul04);
        __m128 Add01 = _mm_add_ps(Sub01, Mul05);
        __m128 Inv1 = _mm_mul_ps(SignA, Add01);

        // col2
        // + (Vec0[0] * Fac1[0] - Vec1[0] * Fac3[0] + Vec3[0] * Fac5[0]),
        // - (Vec0[0] * Fac1[1] - Vec1[1] * Fac3[1] + Vec3[1] * Fac5[1]),
        // + (Vec0[0] * Fac1[2] - Vec1[2] * Fac3[2] + Vec3[2] * Fac5[2]),
        // - (Vec0[0] * Fac1[3] - Vec1[3] * Fac3[3] + Vec3[3] * Fac5[3]),
        __m128 Mul06 = _mm_mul_ps(Vec0, Fac1);
        __m128 Mul07 = _mm_mul_ps(Vec1, Fac3);
        __m128 Mul08 = _mm_mul_ps(Vec3, Fac5);
        __m128 Sub02 = _mm_sub_ps(Mul06, Mul07);
        __m128 Add02 = _mm_add_ps(Sub02, Mul08);
        __m128 Inv2 = _mm_mul_ps(SignB, Add02);

        // col3
        // - (Vec1[0] * Fac2[0] - Vec1[0] * Fac4[0] + Vec2[0] * Fac5[0]),
        // + (Vec1[0] * Fac2[1] - Vec1[1] * Fac4[1] + Vec2[1] * Fac5[1]),
        // - (Vec1[0] * Fac2[2] - Vec1[2] * Fac4[2] + Vec2[2] * Fac5[2]),
        // + (Vec1[0] * Fac2[3] - Vec1[3] * Fac4[3] + Vec2[3] * Fac5[3]));
        __m128 Mul09 = _mm_mul_ps(Vec0, Fac2);
        __m128 Mul10 = _mm_mul_ps(Vec1, Fac4);
        __m128 Mul11 = _mm_mul_ps(Vec2, Fac5);
        __m128 Sub03 = _mm_sub_ps(Mul09, Mul10);
        __m128 Add03 = _mm_add_ps(Sub03, Mul11);
        __m128 Inv3 = _mm_mul_ps(SignA, Add03);

        __m128 Row0 = _mm_shuffle_ps(Inv0, Inv1, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Row1 = _mm_shuffle_ps(Inv2, Inv3, _MM_SHUFFLE(0, 0, 0, 0));
        __m128 Row2 = _mm_shuffle_ps(Row0, Row1, _MM_SHUFFLE(2, 0, 2, 0));

        //	valType Determinant = m[0][0] * Inverse[0][0] 
        //						+ m[0][1] * Inverse[1][0] 
        //						+ m[0][2] * Inverse[2][0] 
        //						+ m[0][3] * Inverse[3][0];
        __m128 Det0 = _mm_dp_ps(mat.mm_arr[0], Row2, 0xff);
        __m128 Rcp0 = _mm_rcp_ps(Det0);
        //__m128 Rcp0 = _mm_div_ps(one, Det0);
        //	Inverse /= Determinant;
        out.mm_arr[0] = _mm_mul_ps(Inv0, Rcp0);
        out.mm_arr[1] = _mm_mul_ps(Inv1, Rcp0);
        out.mm_arr[2] = _mm_mul_ps(Inv2, Rcp0);
        out.mm_arr[3] = _mm_mul_ps(Inv3, Rcp0);

        return out;
    }

    mat4f transpose(const mat4f &mat) noexcept {
        mat4f temp(mat);
        _MM_TRANSPOSE4_PS(temp.mm_row0, temp.mm_row1, temp.mm_row2, temp.mm_row3);

        return temp;
    }
}