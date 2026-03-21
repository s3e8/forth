// BYTECODE(V3ADD, "v3+", 0, 6, 0, {
//     FAT(3) += FAT(0);
//     FAT(4) += FAT(1);
//     FAT(5) += FAT(2);
//     fs+=3;
//   })
// BYTECODE(V3SUB, "v3-", 0, 6, 0, {
//     FAT(3) -= FAT(0);
//     FAT(4) -= FAT(1);
//     FAT(5) -= FAT(2);
//     fs+=3;
//   })
// BYTECODE(V3SCALARMULT, "v3s*", 0, 4, 0, {
//     FAT(1) *= FAT(0);
//     FAT(2) *= FAT(0);
//     FAT(3) *= FAT(0);
//     fs+=1;
//   })
// BYTECODE(V3SCALARDIV, "v3s/", 0, 4, 0, {
//     FAT(1) /= FAT(0);
//     FAT(2) /= FAT(0);
//     FAT(3) /= FAT(0);
//     fs+=1;
//   })
// BYTECODE(V3DOT, "v3dot", 0, 6, 0, {
//     float result = FAT(0)*FAT(3) + FAT(1)*FAT(4) + FAT(2)*FAT(5);
//     fs+=6;
//     FPUSH(result);
//   })
// BYTECODE(V3LENSQUARED, "v3len^2", 0, 3, 0, {
//     float result = FAT(0)*FAT(0) + FAT(1)*FAT(1) + FAT(2)*FAT(2);
//     fs+=3;
//     FPUSH(result);
//   })
// BYTECODE(V3CROSS, "v3cross", 0, 6, 0, {
//     float a1 = FAT(5);
//     float a2 = FAT(4);
//     float a3 = FAT(3);
//     float b1 = FAT(2);
//     float b2 = FAT(1);
//     float b3 = FAT(0);
//     fs += 6;
//     FPUSH(a2*b3 - a3*b2);
//     FPUSH(a3*b1 - a1*b3);
//     FPUSH(a1*b2 - a2*b1);
//   })
// BYTECODE(M33VMUL, "matvec3*", 0, 12, 0, {
//     float a1 = FAT(11)*FAT(2) + FAT(10)*FAT(1) + FAT(9)*FAT(0);
//     float a2 = FAT(8)*FAT(2) + FAT(7)*FAT(1) + FAT(6)*FAT(0);
//     float a3 = FAT(5)*FAT(2) + FAT(4)*FAT(1) + FAT(3)*FAT(0);
//     fs += 12;
//     FPUSH(a1);
//     FPUSH(a2);
//     FPUSH(a3);
//   })
// BYTECODE(M33M33MUL, "matmat*", 0, 18, 0, {
//     // m11 m12 m13 m21 m22 m23 m31 m32 m33
//     // 8   7   6   5   4   3   2   1   0
//     // m11 m12 m13 m21 m22 m23 m31 m32 m33
//     // 17  16  15  14  13  12  11  10  9
//     float m11 = FAT(17)*FAT(8) + FAT(16)*FAT(5) + FAT(15)*FAT(2);
//     float m12 = FAT(17)*FAT(7) + FAT(16)*FAT(4) + FAT(15)*FAT(1);
//     float m13 = FAT(17)*FAT(6) + FAT(16)*FAT(3) + FAT(15)*FAT(0);

//     float m21 = FAT(14)*FAT(8) + FAT(13)*FAT(5) + FAT(12)*FAT(2);
//     float m22 = FAT(14)*FAT(7) + FAT(13)*FAT(4) + FAT(12)*FAT(1);
//     float m23 = FAT(14)*FAT(6) + FAT(13)*FAT(3) + FAT(12)*FAT(0);

//     float m31 = FAT(11)*FAT(8) + FAT(10)*FAT(5) + FAT(9)*FAT(2);
//     float m32 = FAT(11)*FAT(7) + FAT(10)*FAT(4) + FAT(9)*FAT(1);
//     float m33 = FAT(11)*FAT(6) + FAT(10)*FAT(3) + FAT(9)*FAT(0);

//     fs += 9;

//     FAT(8) = m11; FAT(7) = m12; FAT(6) = m13;
//     FAT(5) = m21; FAT(4) = m22; FAT(3) = m23;
//     FAT(2) = m31; FAT(1) = m32; FAT(0) = m33;
//   })
// BYTECODE(STOREV3, "storev3", 1, 3, 0, {
//     float *dst = (float*)POP();
//     *dst++ = FAT(2);
//     *dst++ = FAT(1);
//     *dst = FAT(0);
//     fs += 3;
//   })
// BYTECODE(LOADV3, "loadv3", 1, 0, 0, {
//     float *src = (float*)POP();
//     fs -= 3;
//     FAT(2) = *src++;
//     FAT(1) = *src++;
//     FAT(0) = *src;
//   })