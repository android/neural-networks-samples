/*
 * Copyright (C) 2021 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// This is a generated file.

// Extracted from the TensorFlow Lite PoseNet Android Demo at
// https://github.com/tensorflow/examples/tree/master/lite/examples/posenet/android

#include "NnapiExecutor.h"
#include "NnapiUtils.h"

namespace pose_estimation {

void populatePoseEstimationModel(ANeuralNetworksModel* model, ANeuralNetworksMemory* memory) {
    // Operands
    uint32_t operandIndex = 0;
    ANeuralNetworksOperandType type0;
    type0.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type0.scale = 0;
    type0.zeroPoint = 0;
    type0.dimensionCount = 4;
    uint32_t type0Dims[] = {1, 257, 257, 3};
    type0.dimensions = type0Dims;
    ANeuralNetworksModel_addOperand(model, &type0);
    uint32_t operand0 = operandIndex++;

    ANeuralNetworksOperandType type1;
    type1.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type1.scale = 0;
    type1.zeroPoint = 0;
    type1.dimensionCount = 4;
    uint32_t type1Dims[] = {32, 3, 3, 3};
    type1.dimensions = type1Dims;
    ANeuralNetworksModel_addOperand(model, &type1);
    uint32_t operand1 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand1, memory, 0, 3456);

    ANeuralNetworksOperandType type2;
    type2.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type2.scale = 0;
    type2.zeroPoint = 0;
    type2.dimensionCount = 1;
    uint32_t type2Dims[] = {32};
    type2.dimensions = type2Dims;
    ANeuralNetworksModel_addOperand(model, &type2);
    uint32_t operand2 = operandIndex++;
    uint8_t operand2Value[] = {
            104, 160, 2,   64,  102, 101, 234, 189, 12,  249, 192, 60,  192, 224, 167, 191,
            190, 52,  182, 191, 237, 215, 69,  64,  238, 107, 56,  64,  163, 179, 138, 190,
            237, 43,  186, 191, 3,   33,  66,  191, 162, 180, 65,  63,  65,  253, 73,  64,
            172, 66,  37,  64,  42,  112, 40,  192, 130, 249, 34,  64,  136, 213, 172, 64,
            111, 93,  0,   64,  199, 104, 204, 191, 145, 170, 61,  64,  127, 22,  94,  64,
            194, 87,  124, 64,  168, 249, 70,  64,  4,   216, 179, 191, 66,  138, 236, 192,
            180, 222, 166, 63,  73,  206, 36,  64,  114, 254, 104, 192, 102, 102, 40,  64,
            187, 13,  170, 64,  146, 70,  34,  190, 210, 205, 45,  62,  64,  159, 186, 62};
    ANeuralNetworksModel_setOperandValue(model, operand2, operand2Value, 128);

    ANeuralNetworksOperandType type3;
    type3.type = ANEURALNETWORKS_INT32;
    type3.scale = 0;
    type3.zeroPoint = 0;
    type3.dimensionCount = 0;
    type3.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type3);
    uint32_t operand3 = operandIndex++;
    uint8_t operand3Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand3, operand3Value, 4);

    ANeuralNetworksOperandType type4;
    type4.type = ANEURALNETWORKS_INT32;
    type4.scale = 0;
    type4.zeroPoint = 0;
    type4.dimensionCount = 0;
    type4.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type4);
    uint32_t operand4 = operandIndex++;
    uint8_t operand4Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand4, operand4Value, 4);

    ANeuralNetworksOperandType type5;
    type5.type = ANEURALNETWORKS_INT32;
    type5.scale = 0;
    type5.zeroPoint = 0;
    type5.dimensionCount = 0;
    type5.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type5);
    uint32_t operand5 = operandIndex++;
    uint8_t operand5Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand5, operand5Value, 4);

    ANeuralNetworksOperandType type6;
    type6.type = ANEURALNETWORKS_INT32;
    type6.scale = 0;
    type6.zeroPoint = 0;
    type6.dimensionCount = 0;
    type6.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type6);
    uint32_t operand6 = operandIndex++;
    uint8_t operand6Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand6, operand6Value, 4);

    ANeuralNetworksOperandType type7;
    type7.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type7.scale = 0;
    type7.zeroPoint = 0;
    type7.dimensionCount = 4;
    uint32_t type7Dims[] = {1, 129, 129, 32};
    type7.dimensions = type7Dims;
    ANeuralNetworksModel_addOperand(model, &type7);
    uint32_t operand7 = operandIndex++;

    ANeuralNetworksOperandType type8;
    type8.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type8.scale = 0;
    type8.zeroPoint = 0;
    type8.dimensionCount = 4;
    uint32_t type8Dims[] = {1, 3, 3, 32};
    type8.dimensions = type8Dims;
    ANeuralNetworksModel_addOperand(model, &type8);
    uint32_t operand8 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand8, memory, 3456, 1152);

    ANeuralNetworksOperandType type9;
    type9.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type9.scale = 0;
    type9.zeroPoint = 0;
    type9.dimensionCount = 1;
    uint32_t type9Dims[] = {32};
    type9.dimensions = type9Dims;
    ANeuralNetworksModel_addOperand(model, &type9);
    uint32_t operand9 = operandIndex++;
    uint8_t operand9Value[] = {
            1,   215, 128, 64,  220, 61,  179, 190, 16,  111, 72,  190, 28,  141, 99,  191,
            124, 108, 138, 191, 124, 113, 38,  63,  82,  27,  130, 63,  181, 89,  62,  191,
            97,  0,   183, 64,  4,   190, 155, 191, 246, 27,  73,  63,  96,  61,  184, 63,
            154, 227, 126, 64,  159, 90,  183, 63,  220, 218, 73,  64,  209, 166, 162, 64,
            102, 58,  147, 191, 223, 15,  247, 190, 12,  230, 47,  63,  141, 184, 202, 63,
            121, 28,  51,  64,  130, 34,  26,  64,  65,  250, 113, 191, 216, 130, 139, 64,
            10,  176, 144, 63,  49,  53,  49,  64,  68,  56,  31,  192, 4,   6,   255, 63,
            252, 10,  150, 64,  24,  18,  3,   191, 84,  44,  156, 189, 41,  132, 89,  63};
    ANeuralNetworksModel_setOperandValue(model, operand9, operand9Value, 128);

    ANeuralNetworksOperandType type10;
    type10.type = ANEURALNETWORKS_INT32;
    type10.scale = 0;
    type10.zeroPoint = 0;
    type10.dimensionCount = 0;
    type10.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type10);
    uint32_t operand10 = operandIndex++;
    uint8_t operand10Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand10, operand10Value, 4);

    ANeuralNetworksOperandType type11;
    type11.type = ANEURALNETWORKS_INT32;
    type11.scale = 0;
    type11.zeroPoint = 0;
    type11.dimensionCount = 0;
    type11.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type11);
    uint32_t operand11 = operandIndex++;
    uint8_t operand11Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand11, operand11Value, 4);

    ANeuralNetworksOperandType type12;
    type12.type = ANEURALNETWORKS_INT32;
    type12.scale = 0;
    type12.zeroPoint = 0;
    type12.dimensionCount = 0;
    type12.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type12);
    uint32_t operand12 = operandIndex++;
    uint8_t operand12Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand12, operand12Value, 4);

    ANeuralNetworksOperandType type13;
    type13.type = ANEURALNETWORKS_INT32;
    type13.scale = 0;
    type13.zeroPoint = 0;
    type13.dimensionCount = 0;
    type13.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type13);
    uint32_t operand13 = operandIndex++;
    uint8_t operand13Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand13, operand13Value, 4);

    ANeuralNetworksOperandType type14;
    type14.type = ANEURALNETWORKS_INT32;
    type14.scale = 0;
    type14.zeroPoint = 0;
    type14.dimensionCount = 0;
    type14.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type14);
    uint32_t operand14 = operandIndex++;
    uint8_t operand14Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand14, operand14Value, 4);

    ANeuralNetworksOperandType type15;
    type15.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type15.scale = 0;
    type15.zeroPoint = 0;
    type15.dimensionCount = 4;
    uint32_t type15Dims[] = {1, 129, 129, 32};
    type15.dimensions = type15Dims;
    ANeuralNetworksModel_addOperand(model, &type15);
    uint32_t operand15 = operandIndex++;

    ANeuralNetworksOperandType type16;
    type16.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type16.scale = 0;
    type16.zeroPoint = 0;
    type16.dimensionCount = 4;
    uint32_t type16Dims[] = {64, 1, 1, 32};
    type16.dimensions = type16Dims;
    ANeuralNetworksModel_addOperand(model, &type16);
    uint32_t operand16 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand16, memory, 4608, 8192);

    ANeuralNetworksOperandType type17;
    type17.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type17.scale = 0;
    type17.zeroPoint = 0;
    type17.dimensionCount = 1;
    uint32_t type17Dims[] = {64};
    type17.dimensions = type17Dims;
    ANeuralNetworksModel_addOperand(model, &type17);
    uint32_t operand17 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand17, memory, 12800, 256);

    ANeuralNetworksOperandType type18;
    type18.type = ANEURALNETWORKS_INT32;
    type18.scale = 0;
    type18.zeroPoint = 0;
    type18.dimensionCount = 0;
    type18.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type18);
    uint32_t operand18 = operandIndex++;
    uint8_t operand18Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand18, operand18Value, 4);

    ANeuralNetworksOperandType type19;
    type19.type = ANEURALNETWORKS_INT32;
    type19.scale = 0;
    type19.zeroPoint = 0;
    type19.dimensionCount = 0;
    type19.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type19);
    uint32_t operand19 = operandIndex++;
    uint8_t operand19Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand19, operand19Value, 4);

    ANeuralNetworksOperandType type20;
    type20.type = ANEURALNETWORKS_INT32;
    type20.scale = 0;
    type20.zeroPoint = 0;
    type20.dimensionCount = 0;
    type20.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type20);
    uint32_t operand20 = operandIndex++;
    uint8_t operand20Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand20, operand20Value, 4);

    ANeuralNetworksOperandType type21;
    type21.type = ANEURALNETWORKS_INT32;
    type21.scale = 0;
    type21.zeroPoint = 0;
    type21.dimensionCount = 0;
    type21.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type21);
    uint32_t operand21 = operandIndex++;
    uint8_t operand21Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand21, operand21Value, 4);

    ANeuralNetworksOperandType type22;
    type22.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type22.scale = 0;
    type22.zeroPoint = 0;
    type22.dimensionCount = 4;
    uint32_t type22Dims[] = {1, 129, 129, 64};
    type22.dimensions = type22Dims;
    ANeuralNetworksModel_addOperand(model, &type22);
    uint32_t operand22 = operandIndex++;

    ANeuralNetworksOperandType type23;
    type23.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type23.scale = 0;
    type23.zeroPoint = 0;
    type23.dimensionCount = 4;
    uint32_t type23Dims[] = {1, 3, 3, 64};
    type23.dimensions = type23Dims;
    ANeuralNetworksModel_addOperand(model, &type23);
    uint32_t operand23 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand23, memory, 13056, 2304);

    ANeuralNetworksOperandType type24;
    type24.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type24.scale = 0;
    type24.zeroPoint = 0;
    type24.dimensionCount = 1;
    uint32_t type24Dims[] = {64};
    type24.dimensions = type24Dims;
    ANeuralNetworksModel_addOperand(model, &type24);
    uint32_t operand24 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand24, memory, 15360, 256);

    ANeuralNetworksOperandType type25;
    type25.type = ANEURALNETWORKS_INT32;
    type25.scale = 0;
    type25.zeroPoint = 0;
    type25.dimensionCount = 0;
    type25.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type25);
    uint32_t operand25 = operandIndex++;
    uint8_t operand25Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand25, operand25Value, 4);

    ANeuralNetworksOperandType type26;
    type26.type = ANEURALNETWORKS_INT32;
    type26.scale = 0;
    type26.zeroPoint = 0;
    type26.dimensionCount = 0;
    type26.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type26);
    uint32_t operand26 = operandIndex++;
    uint8_t operand26Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand26, operand26Value, 4);

    ANeuralNetworksOperandType type27;
    type27.type = ANEURALNETWORKS_INT32;
    type27.scale = 0;
    type27.zeroPoint = 0;
    type27.dimensionCount = 0;
    type27.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type27);
    uint32_t operand27 = operandIndex++;
    uint8_t operand27Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand27, operand27Value, 4);

    ANeuralNetworksOperandType type28;
    type28.type = ANEURALNETWORKS_INT32;
    type28.scale = 0;
    type28.zeroPoint = 0;
    type28.dimensionCount = 0;
    type28.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type28);
    uint32_t operand28 = operandIndex++;
    uint8_t operand28Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand28, operand28Value, 4);

    ANeuralNetworksOperandType type29;
    type29.type = ANEURALNETWORKS_INT32;
    type29.scale = 0;
    type29.zeroPoint = 0;
    type29.dimensionCount = 0;
    type29.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type29);
    uint32_t operand29 = operandIndex++;
    uint8_t operand29Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand29, operand29Value, 4);

    ANeuralNetworksOperandType type30;
    type30.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type30.scale = 0;
    type30.zeroPoint = 0;
    type30.dimensionCount = 4;
    uint32_t type30Dims[] = {1, 65, 65, 64};
    type30.dimensions = type30Dims;
    ANeuralNetworksModel_addOperand(model, &type30);
    uint32_t operand30 = operandIndex++;

    ANeuralNetworksOperandType type31;
    type31.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type31.scale = 0;
    type31.zeroPoint = 0;
    type31.dimensionCount = 4;
    uint32_t type31Dims[] = {128, 1, 1, 64};
    type31.dimensions = type31Dims;
    ANeuralNetworksModel_addOperand(model, &type31);
    uint32_t operand31 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand31, memory, 15616, 32768);

    ANeuralNetworksOperandType type32;
    type32.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type32.scale = 0;
    type32.zeroPoint = 0;
    type32.dimensionCount = 1;
    uint32_t type32Dims[] = {128};
    type32.dimensions = type32Dims;
    ANeuralNetworksModel_addOperand(model, &type32);
    uint32_t operand32 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand32, memory, 48384, 512);

    ANeuralNetworksOperandType type33;
    type33.type = ANEURALNETWORKS_INT32;
    type33.scale = 0;
    type33.zeroPoint = 0;
    type33.dimensionCount = 0;
    type33.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type33);
    uint32_t operand33 = operandIndex++;
    uint8_t operand33Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand33, operand33Value, 4);

    ANeuralNetworksOperandType type34;
    type34.type = ANEURALNETWORKS_INT32;
    type34.scale = 0;
    type34.zeroPoint = 0;
    type34.dimensionCount = 0;
    type34.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type34);
    uint32_t operand34 = operandIndex++;
    uint8_t operand34Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand34, operand34Value, 4);

    ANeuralNetworksOperandType type35;
    type35.type = ANEURALNETWORKS_INT32;
    type35.scale = 0;
    type35.zeroPoint = 0;
    type35.dimensionCount = 0;
    type35.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type35);
    uint32_t operand35 = operandIndex++;
    uint8_t operand35Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand35, operand35Value, 4);

    ANeuralNetworksOperandType type36;
    type36.type = ANEURALNETWORKS_INT32;
    type36.scale = 0;
    type36.zeroPoint = 0;
    type36.dimensionCount = 0;
    type36.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type36);
    uint32_t operand36 = operandIndex++;
    uint8_t operand36Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand36, operand36Value, 4);

    ANeuralNetworksOperandType type37;
    type37.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type37.scale = 0;
    type37.zeroPoint = 0;
    type37.dimensionCount = 4;
    uint32_t type37Dims[] = {1, 65, 65, 128};
    type37.dimensions = type37Dims;
    ANeuralNetworksModel_addOperand(model, &type37);
    uint32_t operand37 = operandIndex++;

    ANeuralNetworksOperandType type38;
    type38.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type38.scale = 0;
    type38.zeroPoint = 0;
    type38.dimensionCount = 4;
    uint32_t type38Dims[] = {1, 3, 3, 128};
    type38.dimensions = type38Dims;
    ANeuralNetworksModel_addOperand(model, &type38);
    uint32_t operand38 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand38, memory, 48896, 4608);

    ANeuralNetworksOperandType type39;
    type39.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type39.scale = 0;
    type39.zeroPoint = 0;
    type39.dimensionCount = 1;
    uint32_t type39Dims[] = {128};
    type39.dimensions = type39Dims;
    ANeuralNetworksModel_addOperand(model, &type39);
    uint32_t operand39 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand39, memory, 53504, 512);

    ANeuralNetworksOperandType type40;
    type40.type = ANEURALNETWORKS_INT32;
    type40.scale = 0;
    type40.zeroPoint = 0;
    type40.dimensionCount = 0;
    type40.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type40);
    uint32_t operand40 = operandIndex++;
    uint8_t operand40Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand40, operand40Value, 4);

    ANeuralNetworksOperandType type41;
    type41.type = ANEURALNETWORKS_INT32;
    type41.scale = 0;
    type41.zeroPoint = 0;
    type41.dimensionCount = 0;
    type41.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type41);
    uint32_t operand41 = operandIndex++;
    uint8_t operand41Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand41, operand41Value, 4);

    ANeuralNetworksOperandType type42;
    type42.type = ANEURALNETWORKS_INT32;
    type42.scale = 0;
    type42.zeroPoint = 0;
    type42.dimensionCount = 0;
    type42.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type42);
    uint32_t operand42 = operandIndex++;
    uint8_t operand42Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand42, operand42Value, 4);

    ANeuralNetworksOperandType type43;
    type43.type = ANEURALNETWORKS_INT32;
    type43.scale = 0;
    type43.zeroPoint = 0;
    type43.dimensionCount = 0;
    type43.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type43);
    uint32_t operand43 = operandIndex++;
    uint8_t operand43Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand43, operand43Value, 4);

    ANeuralNetworksOperandType type44;
    type44.type = ANEURALNETWORKS_INT32;
    type44.scale = 0;
    type44.zeroPoint = 0;
    type44.dimensionCount = 0;
    type44.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type44);
    uint32_t operand44 = operandIndex++;
    uint8_t operand44Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand44, operand44Value, 4);

    ANeuralNetworksOperandType type45;
    type45.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type45.scale = 0;
    type45.zeroPoint = 0;
    type45.dimensionCount = 4;
    uint32_t type45Dims[] = {1, 65, 65, 128};
    type45.dimensions = type45Dims;
    ANeuralNetworksModel_addOperand(model, &type45);
    uint32_t operand45 = operandIndex++;

    ANeuralNetworksOperandType type46;
    type46.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type46.scale = 0;
    type46.zeroPoint = 0;
    type46.dimensionCount = 4;
    uint32_t type46Dims[] = {128, 1, 1, 128};
    type46.dimensions = type46Dims;
    ANeuralNetworksModel_addOperand(model, &type46);
    uint32_t operand46 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand46, memory, 54016, 65536);

    ANeuralNetworksOperandType type47;
    type47.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type47.scale = 0;
    type47.zeroPoint = 0;
    type47.dimensionCount = 1;
    uint32_t type47Dims[] = {128};
    type47.dimensions = type47Dims;
    ANeuralNetworksModel_addOperand(model, &type47);
    uint32_t operand47 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand47, memory, 119552, 512);

    ANeuralNetworksOperandType type48;
    type48.type = ANEURALNETWORKS_INT32;
    type48.scale = 0;
    type48.zeroPoint = 0;
    type48.dimensionCount = 0;
    type48.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type48);
    uint32_t operand48 = operandIndex++;
    uint8_t operand48Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand48, operand48Value, 4);

    ANeuralNetworksOperandType type49;
    type49.type = ANEURALNETWORKS_INT32;
    type49.scale = 0;
    type49.zeroPoint = 0;
    type49.dimensionCount = 0;
    type49.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type49);
    uint32_t operand49 = operandIndex++;
    uint8_t operand49Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand49, operand49Value, 4);

    ANeuralNetworksOperandType type50;
    type50.type = ANEURALNETWORKS_INT32;
    type50.scale = 0;
    type50.zeroPoint = 0;
    type50.dimensionCount = 0;
    type50.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type50);
    uint32_t operand50 = operandIndex++;
    uint8_t operand50Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand50, operand50Value, 4);

    ANeuralNetworksOperandType type51;
    type51.type = ANEURALNETWORKS_INT32;
    type51.scale = 0;
    type51.zeroPoint = 0;
    type51.dimensionCount = 0;
    type51.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type51);
    uint32_t operand51 = operandIndex++;
    uint8_t operand51Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand51, operand51Value, 4);

    ANeuralNetworksOperandType type52;
    type52.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type52.scale = 0;
    type52.zeroPoint = 0;
    type52.dimensionCount = 4;
    uint32_t type52Dims[] = {1, 65, 65, 128};
    type52.dimensions = type52Dims;
    ANeuralNetworksModel_addOperand(model, &type52);
    uint32_t operand52 = operandIndex++;

    ANeuralNetworksOperandType type53;
    type53.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type53.scale = 0;
    type53.zeroPoint = 0;
    type53.dimensionCount = 4;
    uint32_t type53Dims[] = {1, 3, 3, 128};
    type53.dimensions = type53Dims;
    ANeuralNetworksModel_addOperand(model, &type53);
    uint32_t operand53 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand53, memory, 120064, 4608);

    ANeuralNetworksOperandType type54;
    type54.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type54.scale = 0;
    type54.zeroPoint = 0;
    type54.dimensionCount = 1;
    uint32_t type54Dims[] = {128};
    type54.dimensions = type54Dims;
    ANeuralNetworksModel_addOperand(model, &type54);
    uint32_t operand54 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand54, memory, 124672, 512);

    ANeuralNetworksOperandType type55;
    type55.type = ANEURALNETWORKS_INT32;
    type55.scale = 0;
    type55.zeroPoint = 0;
    type55.dimensionCount = 0;
    type55.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type55);
    uint32_t operand55 = operandIndex++;
    uint8_t operand55Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand55, operand55Value, 4);

    ANeuralNetworksOperandType type56;
    type56.type = ANEURALNETWORKS_INT32;
    type56.scale = 0;
    type56.zeroPoint = 0;
    type56.dimensionCount = 0;
    type56.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type56);
    uint32_t operand56 = operandIndex++;
    uint8_t operand56Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand56, operand56Value, 4);

    ANeuralNetworksOperandType type57;
    type57.type = ANEURALNETWORKS_INT32;
    type57.scale = 0;
    type57.zeroPoint = 0;
    type57.dimensionCount = 0;
    type57.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type57);
    uint32_t operand57 = operandIndex++;
    uint8_t operand57Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand57, operand57Value, 4);

    ANeuralNetworksOperandType type58;
    type58.type = ANEURALNETWORKS_INT32;
    type58.scale = 0;
    type58.zeroPoint = 0;
    type58.dimensionCount = 0;
    type58.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type58);
    uint32_t operand58 = operandIndex++;
    uint8_t operand58Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand58, operand58Value, 4);

    ANeuralNetworksOperandType type59;
    type59.type = ANEURALNETWORKS_INT32;
    type59.scale = 0;
    type59.zeroPoint = 0;
    type59.dimensionCount = 0;
    type59.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type59);
    uint32_t operand59 = operandIndex++;
    uint8_t operand59Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand59, operand59Value, 4);

    ANeuralNetworksOperandType type60;
    type60.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type60.scale = 0;
    type60.zeroPoint = 0;
    type60.dimensionCount = 4;
    uint32_t type60Dims[] = {1, 33, 33, 128};
    type60.dimensions = type60Dims;
    ANeuralNetworksModel_addOperand(model, &type60);
    uint32_t operand60 = operandIndex++;

    ANeuralNetworksOperandType type61;
    type61.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type61.scale = 0;
    type61.zeroPoint = 0;
    type61.dimensionCount = 4;
    uint32_t type61Dims[] = {256, 1, 1, 128};
    type61.dimensions = type61Dims;
    ANeuralNetworksModel_addOperand(model, &type61);
    uint32_t operand61 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand61, memory, 125184, 131072);

    ANeuralNetworksOperandType type62;
    type62.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type62.scale = 0;
    type62.zeroPoint = 0;
    type62.dimensionCount = 1;
    uint32_t type62Dims[] = {256};
    type62.dimensions = type62Dims;
    ANeuralNetworksModel_addOperand(model, &type62);
    uint32_t operand62 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand62, memory, 256256, 1024);

    ANeuralNetworksOperandType type63;
    type63.type = ANEURALNETWORKS_INT32;
    type63.scale = 0;
    type63.zeroPoint = 0;
    type63.dimensionCount = 0;
    type63.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type63);
    uint32_t operand63 = operandIndex++;
    uint8_t operand63Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand63, operand63Value, 4);

    ANeuralNetworksOperandType type64;
    type64.type = ANEURALNETWORKS_INT32;
    type64.scale = 0;
    type64.zeroPoint = 0;
    type64.dimensionCount = 0;
    type64.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type64);
    uint32_t operand64 = operandIndex++;
    uint8_t operand64Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand64, operand64Value, 4);

    ANeuralNetworksOperandType type65;
    type65.type = ANEURALNETWORKS_INT32;
    type65.scale = 0;
    type65.zeroPoint = 0;
    type65.dimensionCount = 0;
    type65.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type65);
    uint32_t operand65 = operandIndex++;
    uint8_t operand65Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand65, operand65Value, 4);

    ANeuralNetworksOperandType type66;
    type66.type = ANEURALNETWORKS_INT32;
    type66.scale = 0;
    type66.zeroPoint = 0;
    type66.dimensionCount = 0;
    type66.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type66);
    uint32_t operand66 = operandIndex++;
    uint8_t operand66Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand66, operand66Value, 4);

    ANeuralNetworksOperandType type67;
    type67.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type67.scale = 0;
    type67.zeroPoint = 0;
    type67.dimensionCount = 4;
    uint32_t type67Dims[] = {1, 33, 33, 256};
    type67.dimensions = type67Dims;
    ANeuralNetworksModel_addOperand(model, &type67);
    uint32_t operand67 = operandIndex++;

    ANeuralNetworksOperandType type68;
    type68.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type68.scale = 0;
    type68.zeroPoint = 0;
    type68.dimensionCount = 4;
    uint32_t type68Dims[] = {1, 3, 3, 256};
    type68.dimensions = type68Dims;
    ANeuralNetworksModel_addOperand(model, &type68);
    uint32_t operand68 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand68, memory, 257280, 9216);

    ANeuralNetworksOperandType type69;
    type69.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type69.scale = 0;
    type69.zeroPoint = 0;
    type69.dimensionCount = 1;
    uint32_t type69Dims[] = {256};
    type69.dimensions = type69Dims;
    ANeuralNetworksModel_addOperand(model, &type69);
    uint32_t operand69 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand69, memory, 266496, 1024);

    ANeuralNetworksOperandType type70;
    type70.type = ANEURALNETWORKS_INT32;
    type70.scale = 0;
    type70.zeroPoint = 0;
    type70.dimensionCount = 0;
    type70.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type70);
    uint32_t operand70 = operandIndex++;
    uint8_t operand70Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand70, operand70Value, 4);

    ANeuralNetworksOperandType type71;
    type71.type = ANEURALNETWORKS_INT32;
    type71.scale = 0;
    type71.zeroPoint = 0;
    type71.dimensionCount = 0;
    type71.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type71);
    uint32_t operand71 = operandIndex++;
    uint8_t operand71Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand71, operand71Value, 4);

    ANeuralNetworksOperandType type72;
    type72.type = ANEURALNETWORKS_INT32;
    type72.scale = 0;
    type72.zeroPoint = 0;
    type72.dimensionCount = 0;
    type72.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type72);
    uint32_t operand72 = operandIndex++;
    uint8_t operand72Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand72, operand72Value, 4);

    ANeuralNetworksOperandType type73;
    type73.type = ANEURALNETWORKS_INT32;
    type73.scale = 0;
    type73.zeroPoint = 0;
    type73.dimensionCount = 0;
    type73.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type73);
    uint32_t operand73 = operandIndex++;
    uint8_t operand73Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand73, operand73Value, 4);

    ANeuralNetworksOperandType type74;
    type74.type = ANEURALNETWORKS_INT32;
    type74.scale = 0;
    type74.zeroPoint = 0;
    type74.dimensionCount = 0;
    type74.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type74);
    uint32_t operand74 = operandIndex++;
    uint8_t operand74Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand74, operand74Value, 4);

    ANeuralNetworksOperandType type75;
    type75.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type75.scale = 0;
    type75.zeroPoint = 0;
    type75.dimensionCount = 4;
    uint32_t type75Dims[] = {1, 33, 33, 256};
    type75.dimensions = type75Dims;
    ANeuralNetworksModel_addOperand(model, &type75);
    uint32_t operand75 = operandIndex++;

    ANeuralNetworksOperandType type76;
    type76.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type76.scale = 0;
    type76.zeroPoint = 0;
    type76.dimensionCount = 4;
    uint32_t type76Dims[] = {256, 1, 1, 256};
    type76.dimensions = type76Dims;
    ANeuralNetworksModel_addOperand(model, &type76);
    uint32_t operand76 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand76, memory, 267520, 262144);

    ANeuralNetworksOperandType type77;
    type77.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type77.scale = 0;
    type77.zeroPoint = 0;
    type77.dimensionCount = 1;
    uint32_t type77Dims[] = {256};
    type77.dimensions = type77Dims;
    ANeuralNetworksModel_addOperand(model, &type77);
    uint32_t operand77 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand77, memory, 529664, 1024);

    ANeuralNetworksOperandType type78;
    type78.type = ANEURALNETWORKS_INT32;
    type78.scale = 0;
    type78.zeroPoint = 0;
    type78.dimensionCount = 0;
    type78.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type78);
    uint32_t operand78 = operandIndex++;
    uint8_t operand78Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand78, operand78Value, 4);

    ANeuralNetworksOperandType type79;
    type79.type = ANEURALNETWORKS_INT32;
    type79.scale = 0;
    type79.zeroPoint = 0;
    type79.dimensionCount = 0;
    type79.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type79);
    uint32_t operand79 = operandIndex++;
    uint8_t operand79Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand79, operand79Value, 4);

    ANeuralNetworksOperandType type80;
    type80.type = ANEURALNETWORKS_INT32;
    type80.scale = 0;
    type80.zeroPoint = 0;
    type80.dimensionCount = 0;
    type80.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type80);
    uint32_t operand80 = operandIndex++;
    uint8_t operand80Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand80, operand80Value, 4);

    ANeuralNetworksOperandType type81;
    type81.type = ANEURALNETWORKS_INT32;
    type81.scale = 0;
    type81.zeroPoint = 0;
    type81.dimensionCount = 0;
    type81.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type81);
    uint32_t operand81 = operandIndex++;
    uint8_t operand81Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand81, operand81Value, 4);

    ANeuralNetworksOperandType type82;
    type82.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type82.scale = 0;
    type82.zeroPoint = 0;
    type82.dimensionCount = 4;
    uint32_t type82Dims[] = {1, 33, 33, 256};
    type82.dimensions = type82Dims;
    ANeuralNetworksModel_addOperand(model, &type82);
    uint32_t operand82 = operandIndex++;

    ANeuralNetworksOperandType type83;
    type83.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type83.scale = 0;
    type83.zeroPoint = 0;
    type83.dimensionCount = 4;
    uint32_t type83Dims[] = {1, 3, 3, 256};
    type83.dimensions = type83Dims;
    ANeuralNetworksModel_addOperand(model, &type83);
    uint32_t operand83 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand83, memory, 530688, 9216);

    ANeuralNetworksOperandType type84;
    type84.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type84.scale = 0;
    type84.zeroPoint = 0;
    type84.dimensionCount = 1;
    uint32_t type84Dims[] = {256};
    type84.dimensions = type84Dims;
    ANeuralNetworksModel_addOperand(model, &type84);
    uint32_t operand84 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand84, memory, 539904, 1024);

    ANeuralNetworksOperandType type85;
    type85.type = ANEURALNETWORKS_INT32;
    type85.scale = 0;
    type85.zeroPoint = 0;
    type85.dimensionCount = 0;
    type85.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type85);
    uint32_t operand85 = operandIndex++;
    uint8_t operand85Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand85, operand85Value, 4);

    ANeuralNetworksOperandType type86;
    type86.type = ANEURALNETWORKS_INT32;
    type86.scale = 0;
    type86.zeroPoint = 0;
    type86.dimensionCount = 0;
    type86.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type86);
    uint32_t operand86 = operandIndex++;
    uint8_t operand86Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand86, operand86Value, 4);

    ANeuralNetworksOperandType type87;
    type87.type = ANEURALNETWORKS_INT32;
    type87.scale = 0;
    type87.zeroPoint = 0;
    type87.dimensionCount = 0;
    type87.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type87);
    uint32_t operand87 = operandIndex++;
    uint8_t operand87Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand87, operand87Value, 4);

    ANeuralNetworksOperandType type88;
    type88.type = ANEURALNETWORKS_INT32;
    type88.scale = 0;
    type88.zeroPoint = 0;
    type88.dimensionCount = 0;
    type88.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type88);
    uint32_t operand88 = operandIndex++;
    uint8_t operand88Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand88, operand88Value, 4);

    ANeuralNetworksOperandType type89;
    type89.type = ANEURALNETWORKS_INT32;
    type89.scale = 0;
    type89.zeroPoint = 0;
    type89.dimensionCount = 0;
    type89.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type89);
    uint32_t operand89 = operandIndex++;
    uint8_t operand89Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand89, operand89Value, 4);

    ANeuralNetworksOperandType type90;
    type90.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type90.scale = 0;
    type90.zeroPoint = 0;
    type90.dimensionCount = 4;
    uint32_t type90Dims[] = {1, 17, 17, 256};
    type90.dimensions = type90Dims;
    ANeuralNetworksModel_addOperand(model, &type90);
    uint32_t operand90 = operandIndex++;

    ANeuralNetworksOperandType type91;
    type91.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type91.scale = 0;
    type91.zeroPoint = 0;
    type91.dimensionCount = 4;
    uint32_t type91Dims[] = {512, 1, 1, 256};
    type91.dimensions = type91Dims;
    ANeuralNetworksModel_addOperand(model, &type91);
    uint32_t operand91 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand91, memory, 540928, 524288);

    ANeuralNetworksOperandType type92;
    type92.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type92.scale = 0;
    type92.zeroPoint = 0;
    type92.dimensionCount = 1;
    uint32_t type92Dims[] = {512};
    type92.dimensions = type92Dims;
    ANeuralNetworksModel_addOperand(model, &type92);
    uint32_t operand92 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand92, memory, 1065216, 2048);

    ANeuralNetworksOperandType type93;
    type93.type = ANEURALNETWORKS_INT32;
    type93.scale = 0;
    type93.zeroPoint = 0;
    type93.dimensionCount = 0;
    type93.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type93);
    uint32_t operand93 = operandIndex++;
    uint8_t operand93Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand93, operand93Value, 4);

    ANeuralNetworksOperandType type94;
    type94.type = ANEURALNETWORKS_INT32;
    type94.scale = 0;
    type94.zeroPoint = 0;
    type94.dimensionCount = 0;
    type94.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type94);
    uint32_t operand94 = operandIndex++;
    uint8_t operand94Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand94, operand94Value, 4);

    ANeuralNetworksOperandType type95;
    type95.type = ANEURALNETWORKS_INT32;
    type95.scale = 0;
    type95.zeroPoint = 0;
    type95.dimensionCount = 0;
    type95.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type95);
    uint32_t operand95 = operandIndex++;
    uint8_t operand95Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand95, operand95Value, 4);

    ANeuralNetworksOperandType type96;
    type96.type = ANEURALNETWORKS_INT32;
    type96.scale = 0;
    type96.zeroPoint = 0;
    type96.dimensionCount = 0;
    type96.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type96);
    uint32_t operand96 = operandIndex++;
    uint8_t operand96Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand96, operand96Value, 4);

    ANeuralNetworksOperandType type97;
    type97.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type97.scale = 0;
    type97.zeroPoint = 0;
    type97.dimensionCount = 4;
    uint32_t type97Dims[] = {1, 17, 17, 512};
    type97.dimensions = type97Dims;
    ANeuralNetworksModel_addOperand(model, &type97);
    uint32_t operand97 = operandIndex++;

    ANeuralNetworksOperandType type98;
    type98.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type98.scale = 0;
    type98.zeroPoint = 0;
    type98.dimensionCount = 4;
    uint32_t type98Dims[] = {1, 3, 3, 512};
    type98.dimensions = type98Dims;
    ANeuralNetworksModel_addOperand(model, &type98);
    uint32_t operand98 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand98, memory, 1067264, 18432);

    ANeuralNetworksOperandType type99;
    type99.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type99.scale = 0;
    type99.zeroPoint = 0;
    type99.dimensionCount = 1;
    uint32_t type99Dims[] = {512};
    type99.dimensions = type99Dims;
    ANeuralNetworksModel_addOperand(model, &type99);
    uint32_t operand99 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand99, memory, 1085696, 2048);

    ANeuralNetworksOperandType type100;
    type100.type = ANEURALNETWORKS_INT32;
    type100.scale = 0;
    type100.zeroPoint = 0;
    type100.dimensionCount = 0;
    type100.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type100);
    uint32_t operand100 = operandIndex++;
    uint8_t operand100Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand100, operand100Value, 4);

    ANeuralNetworksOperandType type101;
    type101.type = ANEURALNETWORKS_INT32;
    type101.scale = 0;
    type101.zeroPoint = 0;
    type101.dimensionCount = 0;
    type101.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type101);
    uint32_t operand101 = operandIndex++;
    uint8_t operand101Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand101, operand101Value, 4);

    ANeuralNetworksOperandType type102;
    type102.type = ANEURALNETWORKS_INT32;
    type102.scale = 0;
    type102.zeroPoint = 0;
    type102.dimensionCount = 0;
    type102.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type102);
    uint32_t operand102 = operandIndex++;
    uint8_t operand102Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand102, operand102Value, 4);

    ANeuralNetworksOperandType type103;
    type103.type = ANEURALNETWORKS_INT32;
    type103.scale = 0;
    type103.zeroPoint = 0;
    type103.dimensionCount = 0;
    type103.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type103);
    uint32_t operand103 = operandIndex++;
    uint8_t operand103Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand103, operand103Value, 4);

    ANeuralNetworksOperandType type104;
    type104.type = ANEURALNETWORKS_INT32;
    type104.scale = 0;
    type104.zeroPoint = 0;
    type104.dimensionCount = 0;
    type104.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type104);
    uint32_t operand104 = operandIndex++;
    uint8_t operand104Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand104, operand104Value, 4);

    ANeuralNetworksOperandType type105;
    type105.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type105.scale = 0;
    type105.zeroPoint = 0;
    type105.dimensionCount = 4;
    uint32_t type105Dims[] = {1, 17, 17, 512};
    type105.dimensions = type105Dims;
    ANeuralNetworksModel_addOperand(model, &type105);
    uint32_t operand105 = operandIndex++;

    ANeuralNetworksOperandType type106;
    type106.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type106.scale = 0;
    type106.zeroPoint = 0;
    type106.dimensionCount = 4;
    uint32_t type106Dims[] = {512, 1, 1, 512};
    type106.dimensions = type106Dims;
    ANeuralNetworksModel_addOperand(model, &type106);
    uint32_t operand106 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand106, memory, 1087744, 1048576);

    ANeuralNetworksOperandType type107;
    type107.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type107.scale = 0;
    type107.zeroPoint = 0;
    type107.dimensionCount = 1;
    uint32_t type107Dims[] = {512};
    type107.dimensions = type107Dims;
    ANeuralNetworksModel_addOperand(model, &type107);
    uint32_t operand107 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand107, memory, 2136320, 2048);

    ANeuralNetworksOperandType type108;
    type108.type = ANEURALNETWORKS_INT32;
    type108.scale = 0;
    type108.zeroPoint = 0;
    type108.dimensionCount = 0;
    type108.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type108);
    uint32_t operand108 = operandIndex++;
    uint8_t operand108Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand108, operand108Value, 4);

    ANeuralNetworksOperandType type109;
    type109.type = ANEURALNETWORKS_INT32;
    type109.scale = 0;
    type109.zeroPoint = 0;
    type109.dimensionCount = 0;
    type109.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type109);
    uint32_t operand109 = operandIndex++;
    uint8_t operand109Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand109, operand109Value, 4);

    ANeuralNetworksOperandType type110;
    type110.type = ANEURALNETWORKS_INT32;
    type110.scale = 0;
    type110.zeroPoint = 0;
    type110.dimensionCount = 0;
    type110.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type110);
    uint32_t operand110 = operandIndex++;
    uint8_t operand110Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand110, operand110Value, 4);

    ANeuralNetworksOperandType type111;
    type111.type = ANEURALNETWORKS_INT32;
    type111.scale = 0;
    type111.zeroPoint = 0;
    type111.dimensionCount = 0;
    type111.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type111);
    uint32_t operand111 = operandIndex++;
    uint8_t operand111Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand111, operand111Value, 4);

    ANeuralNetworksOperandType type112;
    type112.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type112.scale = 0;
    type112.zeroPoint = 0;
    type112.dimensionCount = 4;
    uint32_t type112Dims[] = {1, 17, 17, 512};
    type112.dimensions = type112Dims;
    ANeuralNetworksModel_addOperand(model, &type112);
    uint32_t operand112 = operandIndex++;

    ANeuralNetworksOperandType type113;
    type113.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type113.scale = 0;
    type113.zeroPoint = 0;
    type113.dimensionCount = 4;
    uint32_t type113Dims[] = {1, 3, 3, 512};
    type113.dimensions = type113Dims;
    ANeuralNetworksModel_addOperand(model, &type113);
    uint32_t operand113 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand113, memory, 2138368, 18432);

    ANeuralNetworksOperandType type114;
    type114.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type114.scale = 0;
    type114.zeroPoint = 0;
    type114.dimensionCount = 1;
    uint32_t type114Dims[] = {512};
    type114.dimensions = type114Dims;
    ANeuralNetworksModel_addOperand(model, &type114);
    uint32_t operand114 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand114, memory, 2156800, 2048);

    ANeuralNetworksOperandType type115;
    type115.type = ANEURALNETWORKS_INT32;
    type115.scale = 0;
    type115.zeroPoint = 0;
    type115.dimensionCount = 0;
    type115.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type115);
    uint32_t operand115 = operandIndex++;
    uint8_t operand115Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand115, operand115Value, 4);

    ANeuralNetworksOperandType type116;
    type116.type = ANEURALNETWORKS_INT32;
    type116.scale = 0;
    type116.zeroPoint = 0;
    type116.dimensionCount = 0;
    type116.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type116);
    uint32_t operand116 = operandIndex++;
    uint8_t operand116Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand116, operand116Value, 4);

    ANeuralNetworksOperandType type117;
    type117.type = ANEURALNETWORKS_INT32;
    type117.scale = 0;
    type117.zeroPoint = 0;
    type117.dimensionCount = 0;
    type117.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type117);
    uint32_t operand117 = operandIndex++;
    uint8_t operand117Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand117, operand117Value, 4);

    ANeuralNetworksOperandType type118;
    type118.type = ANEURALNETWORKS_INT32;
    type118.scale = 0;
    type118.zeroPoint = 0;
    type118.dimensionCount = 0;
    type118.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type118);
    uint32_t operand118 = operandIndex++;
    uint8_t operand118Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand118, operand118Value, 4);

    ANeuralNetworksOperandType type119;
    type119.type = ANEURALNETWORKS_INT32;
    type119.scale = 0;
    type119.zeroPoint = 0;
    type119.dimensionCount = 0;
    type119.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type119);
    uint32_t operand119 = operandIndex++;
    uint8_t operand119Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand119, operand119Value, 4);

    ANeuralNetworksOperandType type120;
    type120.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type120.scale = 0;
    type120.zeroPoint = 0;
    type120.dimensionCount = 4;
    uint32_t type120Dims[] = {1, 17, 17, 512};
    type120.dimensions = type120Dims;
    ANeuralNetworksModel_addOperand(model, &type120);
    uint32_t operand120 = operandIndex++;

    ANeuralNetworksOperandType type121;
    type121.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type121.scale = 0;
    type121.zeroPoint = 0;
    type121.dimensionCount = 4;
    uint32_t type121Dims[] = {512, 1, 1, 512};
    type121.dimensions = type121Dims;
    ANeuralNetworksModel_addOperand(model, &type121);
    uint32_t operand121 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand121, memory, 2158848, 1048576);

    ANeuralNetworksOperandType type122;
    type122.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type122.scale = 0;
    type122.zeroPoint = 0;
    type122.dimensionCount = 1;
    uint32_t type122Dims[] = {512};
    type122.dimensions = type122Dims;
    ANeuralNetworksModel_addOperand(model, &type122);
    uint32_t operand122 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand122, memory, 3207424, 2048);

    ANeuralNetworksOperandType type123;
    type123.type = ANEURALNETWORKS_INT32;
    type123.scale = 0;
    type123.zeroPoint = 0;
    type123.dimensionCount = 0;
    type123.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type123);
    uint32_t operand123 = operandIndex++;
    uint8_t operand123Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand123, operand123Value, 4);

    ANeuralNetworksOperandType type124;
    type124.type = ANEURALNETWORKS_INT32;
    type124.scale = 0;
    type124.zeroPoint = 0;
    type124.dimensionCount = 0;
    type124.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type124);
    uint32_t operand124 = operandIndex++;
    uint8_t operand124Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand124, operand124Value, 4);

    ANeuralNetworksOperandType type125;
    type125.type = ANEURALNETWORKS_INT32;
    type125.scale = 0;
    type125.zeroPoint = 0;
    type125.dimensionCount = 0;
    type125.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type125);
    uint32_t operand125 = operandIndex++;
    uint8_t operand125Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand125, operand125Value, 4);

    ANeuralNetworksOperandType type126;
    type126.type = ANEURALNETWORKS_INT32;
    type126.scale = 0;
    type126.zeroPoint = 0;
    type126.dimensionCount = 0;
    type126.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type126);
    uint32_t operand126 = operandIndex++;
    uint8_t operand126Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand126, operand126Value, 4);

    ANeuralNetworksOperandType type127;
    type127.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type127.scale = 0;
    type127.zeroPoint = 0;
    type127.dimensionCount = 4;
    uint32_t type127Dims[] = {1, 17, 17, 512};
    type127.dimensions = type127Dims;
    ANeuralNetworksModel_addOperand(model, &type127);
    uint32_t operand127 = operandIndex++;

    ANeuralNetworksOperandType type128;
    type128.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type128.scale = 0;
    type128.zeroPoint = 0;
    type128.dimensionCount = 4;
    uint32_t type128Dims[] = {1, 3, 3, 512};
    type128.dimensions = type128Dims;
    ANeuralNetworksModel_addOperand(model, &type128);
    uint32_t operand128 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand128, memory, 3209472, 18432);

    ANeuralNetworksOperandType type129;
    type129.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type129.scale = 0;
    type129.zeroPoint = 0;
    type129.dimensionCount = 1;
    uint32_t type129Dims[] = {512};
    type129.dimensions = type129Dims;
    ANeuralNetworksModel_addOperand(model, &type129);
    uint32_t operand129 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand129, memory, 3227904, 2048);

    ANeuralNetworksOperandType type130;
    type130.type = ANEURALNETWORKS_INT32;
    type130.scale = 0;
    type130.zeroPoint = 0;
    type130.dimensionCount = 0;
    type130.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type130);
    uint32_t operand130 = operandIndex++;
    uint8_t operand130Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand130, operand130Value, 4);

    ANeuralNetworksOperandType type131;
    type131.type = ANEURALNETWORKS_INT32;
    type131.scale = 0;
    type131.zeroPoint = 0;
    type131.dimensionCount = 0;
    type131.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type131);
    uint32_t operand131 = operandIndex++;
    uint8_t operand131Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand131, operand131Value, 4);

    ANeuralNetworksOperandType type132;
    type132.type = ANEURALNETWORKS_INT32;
    type132.scale = 0;
    type132.zeroPoint = 0;
    type132.dimensionCount = 0;
    type132.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type132);
    uint32_t operand132 = operandIndex++;
    uint8_t operand132Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand132, operand132Value, 4);

    ANeuralNetworksOperandType type133;
    type133.type = ANEURALNETWORKS_INT32;
    type133.scale = 0;
    type133.zeroPoint = 0;
    type133.dimensionCount = 0;
    type133.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type133);
    uint32_t operand133 = operandIndex++;
    uint8_t operand133Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand133, operand133Value, 4);

    ANeuralNetworksOperandType type134;
    type134.type = ANEURALNETWORKS_INT32;
    type134.scale = 0;
    type134.zeroPoint = 0;
    type134.dimensionCount = 0;
    type134.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type134);
    uint32_t operand134 = operandIndex++;
    uint8_t operand134Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand134, operand134Value, 4);

    ANeuralNetworksOperandType type135;
    type135.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type135.scale = 0;
    type135.zeroPoint = 0;
    type135.dimensionCount = 4;
    uint32_t type135Dims[] = {1, 17, 17, 512};
    type135.dimensions = type135Dims;
    ANeuralNetworksModel_addOperand(model, &type135);
    uint32_t operand135 = operandIndex++;

    ANeuralNetworksOperandType type136;
    type136.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type136.scale = 0;
    type136.zeroPoint = 0;
    type136.dimensionCount = 4;
    uint32_t type136Dims[] = {512, 1, 1, 512};
    type136.dimensions = type136Dims;
    ANeuralNetworksModel_addOperand(model, &type136);
    uint32_t operand136 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand136, memory, 3229952, 1048576);

    ANeuralNetworksOperandType type137;
    type137.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type137.scale = 0;
    type137.zeroPoint = 0;
    type137.dimensionCount = 1;
    uint32_t type137Dims[] = {512};
    type137.dimensions = type137Dims;
    ANeuralNetworksModel_addOperand(model, &type137);
    uint32_t operand137 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand137, memory, 4278528, 2048);

    ANeuralNetworksOperandType type138;
    type138.type = ANEURALNETWORKS_INT32;
    type138.scale = 0;
    type138.zeroPoint = 0;
    type138.dimensionCount = 0;
    type138.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type138);
    uint32_t operand138 = operandIndex++;
    uint8_t operand138Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand138, operand138Value, 4);

    ANeuralNetworksOperandType type139;
    type139.type = ANEURALNETWORKS_INT32;
    type139.scale = 0;
    type139.zeroPoint = 0;
    type139.dimensionCount = 0;
    type139.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type139);
    uint32_t operand139 = operandIndex++;
    uint8_t operand139Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand139, operand139Value, 4);

    ANeuralNetworksOperandType type140;
    type140.type = ANEURALNETWORKS_INT32;
    type140.scale = 0;
    type140.zeroPoint = 0;
    type140.dimensionCount = 0;
    type140.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type140);
    uint32_t operand140 = operandIndex++;
    uint8_t operand140Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand140, operand140Value, 4);

    ANeuralNetworksOperandType type141;
    type141.type = ANEURALNETWORKS_INT32;
    type141.scale = 0;
    type141.zeroPoint = 0;
    type141.dimensionCount = 0;
    type141.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type141);
    uint32_t operand141 = operandIndex++;
    uint8_t operand141Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand141, operand141Value, 4);

    ANeuralNetworksOperandType type142;
    type142.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type142.scale = 0;
    type142.zeroPoint = 0;
    type142.dimensionCount = 4;
    uint32_t type142Dims[] = {1, 17, 17, 512};
    type142.dimensions = type142Dims;
    ANeuralNetworksModel_addOperand(model, &type142);
    uint32_t operand142 = operandIndex++;

    ANeuralNetworksOperandType type143;
    type143.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type143.scale = 0;
    type143.zeroPoint = 0;
    type143.dimensionCount = 4;
    uint32_t type143Dims[] = {1, 3, 3, 512};
    type143.dimensions = type143Dims;
    ANeuralNetworksModel_addOperand(model, &type143);
    uint32_t operand143 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand143, memory, 4280576, 18432);

    ANeuralNetworksOperandType type144;
    type144.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type144.scale = 0;
    type144.zeroPoint = 0;
    type144.dimensionCount = 1;
    uint32_t type144Dims[] = {512};
    type144.dimensions = type144Dims;
    ANeuralNetworksModel_addOperand(model, &type144);
    uint32_t operand144 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand144, memory, 4299008, 2048);

    ANeuralNetworksOperandType type145;
    type145.type = ANEURALNETWORKS_INT32;
    type145.scale = 0;
    type145.zeroPoint = 0;
    type145.dimensionCount = 0;
    type145.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type145);
    uint32_t operand145 = operandIndex++;
    uint8_t operand145Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand145, operand145Value, 4);

    ANeuralNetworksOperandType type146;
    type146.type = ANEURALNETWORKS_INT32;
    type146.scale = 0;
    type146.zeroPoint = 0;
    type146.dimensionCount = 0;
    type146.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type146);
    uint32_t operand146 = operandIndex++;
    uint8_t operand146Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand146, operand146Value, 4);

    ANeuralNetworksOperandType type147;
    type147.type = ANEURALNETWORKS_INT32;
    type147.scale = 0;
    type147.zeroPoint = 0;
    type147.dimensionCount = 0;
    type147.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type147);
    uint32_t operand147 = operandIndex++;
    uint8_t operand147Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand147, operand147Value, 4);

    ANeuralNetworksOperandType type148;
    type148.type = ANEURALNETWORKS_INT32;
    type148.scale = 0;
    type148.zeroPoint = 0;
    type148.dimensionCount = 0;
    type148.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type148);
    uint32_t operand148 = operandIndex++;
    uint8_t operand148Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand148, operand148Value, 4);

    ANeuralNetworksOperandType type149;
    type149.type = ANEURALNETWORKS_INT32;
    type149.scale = 0;
    type149.zeroPoint = 0;
    type149.dimensionCount = 0;
    type149.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type149);
    uint32_t operand149 = operandIndex++;
    uint8_t operand149Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand149, operand149Value, 4);

    ANeuralNetworksOperandType type150;
    type150.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type150.scale = 0;
    type150.zeroPoint = 0;
    type150.dimensionCount = 4;
    uint32_t type150Dims[] = {1, 17, 17, 512};
    type150.dimensions = type150Dims;
    ANeuralNetworksModel_addOperand(model, &type150);
    uint32_t operand150 = operandIndex++;

    ANeuralNetworksOperandType type151;
    type151.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type151.scale = 0;
    type151.zeroPoint = 0;
    type151.dimensionCount = 4;
    uint32_t type151Dims[] = {512, 1, 1, 512};
    type151.dimensions = type151Dims;
    ANeuralNetworksModel_addOperand(model, &type151);
    uint32_t operand151 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand151, memory, 4301056, 1048576);

    ANeuralNetworksOperandType type152;
    type152.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type152.scale = 0;
    type152.zeroPoint = 0;
    type152.dimensionCount = 1;
    uint32_t type152Dims[] = {512};
    type152.dimensions = type152Dims;
    ANeuralNetworksModel_addOperand(model, &type152);
    uint32_t operand152 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand152, memory, 5349632, 2048);

    ANeuralNetworksOperandType type153;
    type153.type = ANEURALNETWORKS_INT32;
    type153.scale = 0;
    type153.zeroPoint = 0;
    type153.dimensionCount = 0;
    type153.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type153);
    uint32_t operand153 = operandIndex++;
    uint8_t operand153Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand153, operand153Value, 4);

    ANeuralNetworksOperandType type154;
    type154.type = ANEURALNETWORKS_INT32;
    type154.scale = 0;
    type154.zeroPoint = 0;
    type154.dimensionCount = 0;
    type154.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type154);
    uint32_t operand154 = operandIndex++;
    uint8_t operand154Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand154, operand154Value, 4);

    ANeuralNetworksOperandType type155;
    type155.type = ANEURALNETWORKS_INT32;
    type155.scale = 0;
    type155.zeroPoint = 0;
    type155.dimensionCount = 0;
    type155.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type155);
    uint32_t operand155 = operandIndex++;
    uint8_t operand155Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand155, operand155Value, 4);

    ANeuralNetworksOperandType type156;
    type156.type = ANEURALNETWORKS_INT32;
    type156.scale = 0;
    type156.zeroPoint = 0;
    type156.dimensionCount = 0;
    type156.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type156);
    uint32_t operand156 = operandIndex++;
    uint8_t operand156Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand156, operand156Value, 4);

    ANeuralNetworksOperandType type157;
    type157.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type157.scale = 0;
    type157.zeroPoint = 0;
    type157.dimensionCount = 4;
    uint32_t type157Dims[] = {1, 17, 17, 512};
    type157.dimensions = type157Dims;
    ANeuralNetworksModel_addOperand(model, &type157);
    uint32_t operand157 = operandIndex++;

    ANeuralNetworksOperandType type158;
    type158.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type158.scale = 0;
    type158.zeroPoint = 0;
    type158.dimensionCount = 4;
    uint32_t type158Dims[] = {1, 3, 3, 512};
    type158.dimensions = type158Dims;
    ANeuralNetworksModel_addOperand(model, &type158);
    uint32_t operand158 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand158, memory, 5351680, 18432);

    ANeuralNetworksOperandType type159;
    type159.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type159.scale = 0;
    type159.zeroPoint = 0;
    type159.dimensionCount = 1;
    uint32_t type159Dims[] = {512};
    type159.dimensions = type159Dims;
    ANeuralNetworksModel_addOperand(model, &type159);
    uint32_t operand159 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand159, memory, 5370112, 2048);

    ANeuralNetworksOperandType type160;
    type160.type = ANEURALNETWORKS_INT32;
    type160.scale = 0;
    type160.zeroPoint = 0;
    type160.dimensionCount = 0;
    type160.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type160);
    uint32_t operand160 = operandIndex++;
    uint8_t operand160Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand160, operand160Value, 4);

    ANeuralNetworksOperandType type161;
    type161.type = ANEURALNETWORKS_INT32;
    type161.scale = 0;
    type161.zeroPoint = 0;
    type161.dimensionCount = 0;
    type161.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type161);
    uint32_t operand161 = operandIndex++;
    uint8_t operand161Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand161, operand161Value, 4);

    ANeuralNetworksOperandType type162;
    type162.type = ANEURALNETWORKS_INT32;
    type162.scale = 0;
    type162.zeroPoint = 0;
    type162.dimensionCount = 0;
    type162.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type162);
    uint32_t operand162 = operandIndex++;
    uint8_t operand162Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand162, operand162Value, 4);

    ANeuralNetworksOperandType type163;
    type163.type = ANEURALNETWORKS_INT32;
    type163.scale = 0;
    type163.zeroPoint = 0;
    type163.dimensionCount = 0;
    type163.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type163);
    uint32_t operand163 = operandIndex++;
    uint8_t operand163Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand163, operand163Value, 4);

    ANeuralNetworksOperandType type164;
    type164.type = ANEURALNETWORKS_INT32;
    type164.scale = 0;
    type164.zeroPoint = 0;
    type164.dimensionCount = 0;
    type164.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type164);
    uint32_t operand164 = operandIndex++;
    uint8_t operand164Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand164, operand164Value, 4);

    ANeuralNetworksOperandType type165;
    type165.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type165.scale = 0;
    type165.zeroPoint = 0;
    type165.dimensionCount = 4;
    uint32_t type165Dims[] = {1, 17, 17, 512};
    type165.dimensions = type165Dims;
    ANeuralNetworksModel_addOperand(model, &type165);
    uint32_t operand165 = operandIndex++;

    ANeuralNetworksOperandType type166;
    type166.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type166.scale = 0;
    type166.zeroPoint = 0;
    type166.dimensionCount = 4;
    uint32_t type166Dims[] = {512, 1, 1, 512};
    type166.dimensions = type166Dims;
    ANeuralNetworksModel_addOperand(model, &type166);
    uint32_t operand166 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand166, memory, 5372160, 1048576);

    ANeuralNetworksOperandType type167;
    type167.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type167.scale = 0;
    type167.zeroPoint = 0;
    type167.dimensionCount = 1;
    uint32_t type167Dims[] = {512};
    type167.dimensions = type167Dims;
    ANeuralNetworksModel_addOperand(model, &type167);
    uint32_t operand167 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand167, memory, 6420736, 2048);

    ANeuralNetworksOperandType type168;
    type168.type = ANEURALNETWORKS_INT32;
    type168.scale = 0;
    type168.zeroPoint = 0;
    type168.dimensionCount = 0;
    type168.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type168);
    uint32_t operand168 = operandIndex++;
    uint8_t operand168Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand168, operand168Value, 4);

    ANeuralNetworksOperandType type169;
    type169.type = ANEURALNETWORKS_INT32;
    type169.scale = 0;
    type169.zeroPoint = 0;
    type169.dimensionCount = 0;
    type169.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type169);
    uint32_t operand169 = operandIndex++;
    uint8_t operand169Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand169, operand169Value, 4);

    ANeuralNetworksOperandType type170;
    type170.type = ANEURALNETWORKS_INT32;
    type170.scale = 0;
    type170.zeroPoint = 0;
    type170.dimensionCount = 0;
    type170.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type170);
    uint32_t operand170 = operandIndex++;
    uint8_t operand170Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand170, operand170Value, 4);

    ANeuralNetworksOperandType type171;
    type171.type = ANEURALNETWORKS_INT32;
    type171.scale = 0;
    type171.zeroPoint = 0;
    type171.dimensionCount = 0;
    type171.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type171);
    uint32_t operand171 = operandIndex++;
    uint8_t operand171Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand171, operand171Value, 4);

    ANeuralNetworksOperandType type172;
    type172.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type172.scale = 0;
    type172.zeroPoint = 0;
    type172.dimensionCount = 4;
    uint32_t type172Dims[] = {1, 17, 17, 512};
    type172.dimensions = type172Dims;
    ANeuralNetworksModel_addOperand(model, &type172);
    uint32_t operand172 = operandIndex++;

    ANeuralNetworksOperandType type173;
    type173.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type173.scale = 0;
    type173.zeroPoint = 0;
    type173.dimensionCount = 4;
    uint32_t type173Dims[] = {1, 3, 3, 512};
    type173.dimensions = type173Dims;
    ANeuralNetworksModel_addOperand(model, &type173);
    uint32_t operand173 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand173, memory, 6422784, 18432);

    ANeuralNetworksOperandType type174;
    type174.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type174.scale = 0;
    type174.zeroPoint = 0;
    type174.dimensionCount = 1;
    uint32_t type174Dims[] = {512};
    type174.dimensions = type174Dims;
    ANeuralNetworksModel_addOperand(model, &type174);
    uint32_t operand174 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand174, memory, 6441216, 2048);

    ANeuralNetworksOperandType type175;
    type175.type = ANEURALNETWORKS_INT32;
    type175.scale = 0;
    type175.zeroPoint = 0;
    type175.dimensionCount = 0;
    type175.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type175);
    uint32_t operand175 = operandIndex++;
    uint8_t operand175Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand175, operand175Value, 4);

    ANeuralNetworksOperandType type176;
    type176.type = ANEURALNETWORKS_INT32;
    type176.scale = 0;
    type176.zeroPoint = 0;
    type176.dimensionCount = 0;
    type176.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type176);
    uint32_t operand176 = operandIndex++;
    uint8_t operand176Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand176, operand176Value, 4);

    ANeuralNetworksOperandType type177;
    type177.type = ANEURALNETWORKS_INT32;
    type177.scale = 0;
    type177.zeroPoint = 0;
    type177.dimensionCount = 0;
    type177.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type177);
    uint32_t operand177 = operandIndex++;
    uint8_t operand177Value[] = {2, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand177, operand177Value, 4);

    ANeuralNetworksOperandType type178;
    type178.type = ANEURALNETWORKS_INT32;
    type178.scale = 0;
    type178.zeroPoint = 0;
    type178.dimensionCount = 0;
    type178.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type178);
    uint32_t operand178 = operandIndex++;
    uint8_t operand178Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand178, operand178Value, 4);

    ANeuralNetworksOperandType type179;
    type179.type = ANEURALNETWORKS_INT32;
    type179.scale = 0;
    type179.zeroPoint = 0;
    type179.dimensionCount = 0;
    type179.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type179);
    uint32_t operand179 = operandIndex++;
    uint8_t operand179Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand179, operand179Value, 4);

    ANeuralNetworksOperandType type180;
    type180.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type180.scale = 0;
    type180.zeroPoint = 0;
    type180.dimensionCount = 4;
    uint32_t type180Dims[] = {1, 9, 9, 512};
    type180.dimensions = type180Dims;
    ANeuralNetworksModel_addOperand(model, &type180);
    uint32_t operand180 = operandIndex++;

    ANeuralNetworksOperandType type181;
    type181.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type181.scale = 0;
    type181.zeroPoint = 0;
    type181.dimensionCount = 4;
    uint32_t type181Dims[] = {1024, 1, 1, 512};
    type181.dimensions = type181Dims;
    ANeuralNetworksModel_addOperand(model, &type181);
    uint32_t operand181 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand181, memory, 6443264, 2097152);

    ANeuralNetworksOperandType type182;
    type182.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type182.scale = 0;
    type182.zeroPoint = 0;
    type182.dimensionCount = 1;
    uint32_t type182Dims[] = {1024};
    type182.dimensions = type182Dims;
    ANeuralNetworksModel_addOperand(model, &type182);
    uint32_t operand182 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand182, memory, 8540416, 4096);

    ANeuralNetworksOperandType type183;
    type183.type = ANEURALNETWORKS_INT32;
    type183.scale = 0;
    type183.zeroPoint = 0;
    type183.dimensionCount = 0;
    type183.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type183);
    uint32_t operand183 = operandIndex++;
    uint8_t operand183Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand183, operand183Value, 4);

    ANeuralNetworksOperandType type184;
    type184.type = ANEURALNETWORKS_INT32;
    type184.scale = 0;
    type184.zeroPoint = 0;
    type184.dimensionCount = 0;
    type184.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type184);
    uint32_t operand184 = operandIndex++;
    uint8_t operand184Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand184, operand184Value, 4);

    ANeuralNetworksOperandType type185;
    type185.type = ANEURALNETWORKS_INT32;
    type185.scale = 0;
    type185.zeroPoint = 0;
    type185.dimensionCount = 0;
    type185.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type185);
    uint32_t operand185 = operandIndex++;
    uint8_t operand185Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand185, operand185Value, 4);

    ANeuralNetworksOperandType type186;
    type186.type = ANEURALNETWORKS_INT32;
    type186.scale = 0;
    type186.zeroPoint = 0;
    type186.dimensionCount = 0;
    type186.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type186);
    uint32_t operand186 = operandIndex++;
    uint8_t operand186Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand186, operand186Value, 4);

    ANeuralNetworksOperandType type187;
    type187.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type187.scale = 0;
    type187.zeroPoint = 0;
    type187.dimensionCount = 4;
    uint32_t type187Dims[] = {1, 9, 9, 1024};
    type187.dimensions = type187Dims;
    ANeuralNetworksModel_addOperand(model, &type187);
    uint32_t operand187 = operandIndex++;

    ANeuralNetworksOperandType type188;
    type188.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type188.scale = 0;
    type188.zeroPoint = 0;
    type188.dimensionCount = 4;
    uint32_t type188Dims[] = {1, 3, 3, 1024};
    type188.dimensions = type188Dims;
    ANeuralNetworksModel_addOperand(model, &type188);
    uint32_t operand188 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand188, memory, 8544512, 36864);

    ANeuralNetworksOperandType type189;
    type189.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type189.scale = 0;
    type189.zeroPoint = 0;
    type189.dimensionCount = 1;
    uint32_t type189Dims[] = {1024};
    type189.dimensions = type189Dims;
    ANeuralNetworksModel_addOperand(model, &type189);
    uint32_t operand189 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand189, memory, 8581376, 4096);

    ANeuralNetworksOperandType type190;
    type190.type = ANEURALNETWORKS_INT32;
    type190.scale = 0;
    type190.zeroPoint = 0;
    type190.dimensionCount = 0;
    type190.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type190);
    uint32_t operand190 = operandIndex++;
    uint8_t operand190Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand190, operand190Value, 4);

    ANeuralNetworksOperandType type191;
    type191.type = ANEURALNETWORKS_INT32;
    type191.scale = 0;
    type191.zeroPoint = 0;
    type191.dimensionCount = 0;
    type191.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type191);
    uint32_t operand191 = operandIndex++;
    uint8_t operand191Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand191, operand191Value, 4);

    ANeuralNetworksOperandType type192;
    type192.type = ANEURALNETWORKS_INT32;
    type192.scale = 0;
    type192.zeroPoint = 0;
    type192.dimensionCount = 0;
    type192.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type192);
    uint32_t operand192 = operandIndex++;
    uint8_t operand192Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand192, operand192Value, 4);

    ANeuralNetworksOperandType type193;
    type193.type = ANEURALNETWORKS_INT32;
    type193.scale = 0;
    type193.zeroPoint = 0;
    type193.dimensionCount = 0;
    type193.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type193);
    uint32_t operand193 = operandIndex++;
    uint8_t operand193Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand193, operand193Value, 4);

    ANeuralNetworksOperandType type194;
    type194.type = ANEURALNETWORKS_INT32;
    type194.scale = 0;
    type194.zeroPoint = 0;
    type194.dimensionCount = 0;
    type194.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type194);
    uint32_t operand194 = operandIndex++;
    uint8_t operand194Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand194, operand194Value, 4);

    ANeuralNetworksOperandType type195;
    type195.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type195.scale = 0;
    type195.zeroPoint = 0;
    type195.dimensionCount = 4;
    uint32_t type195Dims[] = {1, 9, 9, 1024};
    type195.dimensions = type195Dims;
    ANeuralNetworksModel_addOperand(model, &type195);
    uint32_t operand195 = operandIndex++;

    ANeuralNetworksOperandType type196;
    type196.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type196.scale = 0;
    type196.zeroPoint = 0;
    type196.dimensionCount = 4;
    uint32_t type196Dims[] = {1024, 1, 1, 1024};
    type196.dimensions = type196Dims;
    ANeuralNetworksModel_addOperand(model, &type196);
    uint32_t operand196 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand196, memory, 8585472, 4194304);

    ANeuralNetworksOperandType type197;
    type197.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type197.scale = 0;
    type197.zeroPoint = 0;
    type197.dimensionCount = 1;
    uint32_t type197Dims[] = {1024};
    type197.dimensions = type197Dims;
    ANeuralNetworksModel_addOperand(model, &type197);
    uint32_t operand197 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand197, memory, 12779776, 4096);

    ANeuralNetworksOperandType type198;
    type198.type = ANEURALNETWORKS_INT32;
    type198.scale = 0;
    type198.zeroPoint = 0;
    type198.dimensionCount = 0;
    type198.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type198);
    uint32_t operand198 = operandIndex++;
    uint8_t operand198Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand198, operand198Value, 4);

    ANeuralNetworksOperandType type199;
    type199.type = ANEURALNETWORKS_INT32;
    type199.scale = 0;
    type199.zeroPoint = 0;
    type199.dimensionCount = 0;
    type199.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type199);
    uint32_t operand199 = operandIndex++;
    uint8_t operand199Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand199, operand199Value, 4);

    ANeuralNetworksOperandType type200;
    type200.type = ANEURALNETWORKS_INT32;
    type200.scale = 0;
    type200.zeroPoint = 0;
    type200.dimensionCount = 0;
    type200.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type200);
    uint32_t operand200 = operandIndex++;
    uint8_t operand200Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand200, operand200Value, 4);

    ANeuralNetworksOperandType type201;
    type201.type = ANEURALNETWORKS_INT32;
    type201.scale = 0;
    type201.zeroPoint = 0;
    type201.dimensionCount = 0;
    type201.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type201);
    uint32_t operand201 = operandIndex++;
    uint8_t operand201Value[] = {3, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand201, operand201Value, 4);

    ANeuralNetworksOperandType type202;
    type202.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type202.scale = 0;
    type202.zeroPoint = 0;
    type202.dimensionCount = 4;
    uint32_t type202Dims[] = {1, 9, 9, 1024};
    type202.dimensions = type202Dims;
    ANeuralNetworksModel_addOperand(model, &type202);
    uint32_t operand202 = operandIndex++;

    ANeuralNetworksOperandType type203;
    type203.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type203.scale = 0;
    type203.zeroPoint = 0;
    type203.dimensionCount = 4;
    uint32_t type203Dims[] = {17, 1, 1, 1024};
    type203.dimensions = type203Dims;
    ANeuralNetworksModel_addOperand(model, &type203);
    uint32_t operand203 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand203, memory, 12783872, 69632);

    ANeuralNetworksOperandType type204;
    type204.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type204.scale = 0;
    type204.zeroPoint = 0;
    type204.dimensionCount = 1;
    uint32_t type204Dims[] = {17};
    type204.dimensions = type204Dims;
    ANeuralNetworksModel_addOperand(model, &type204);
    uint32_t operand204 = operandIndex++;
    uint8_t operand204Value[] = {
            30,  71,  134, 191, 166, 113, 125, 191, 230, 14,  122, 191, 221, 47,  132, 191, 174,
            46,  132, 191, 138, 48,  163, 191, 193, 114, 156, 191, 85,  107, 182, 191, 190, 107,
            181, 191, 158, 52,  183, 191, 8,   47,  180, 191, 222, 242, 171, 191, 49,  159, 169,
            191, 26,  155, 175, 191, 85,  236, 175, 191, 170, 43,  167, 191, 220, 247, 168, 191};
    ANeuralNetworksModel_setOperandValue(model, operand204, operand204Value, 68);

    ANeuralNetworksOperandType type205;
    type205.type = ANEURALNETWORKS_INT32;
    type205.scale = 0;
    type205.zeroPoint = 0;
    type205.dimensionCount = 0;
    type205.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type205);
    uint32_t operand205 = operandIndex++;
    uint8_t operand205Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand205, operand205Value, 4);

    ANeuralNetworksOperandType type206;
    type206.type = ANEURALNETWORKS_INT32;
    type206.scale = 0;
    type206.zeroPoint = 0;
    type206.dimensionCount = 0;
    type206.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type206);
    uint32_t operand206 = operandIndex++;
    uint8_t operand206Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand206, operand206Value, 4);

    ANeuralNetworksOperandType type207;
    type207.type = ANEURALNETWORKS_INT32;
    type207.scale = 0;
    type207.zeroPoint = 0;
    type207.dimensionCount = 0;
    type207.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type207);
    uint32_t operand207 = operandIndex++;
    uint8_t operand207Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand207, operand207Value, 4);

    ANeuralNetworksOperandType type208;
    type208.type = ANEURALNETWORKS_INT32;
    type208.scale = 0;
    type208.zeroPoint = 0;
    type208.dimensionCount = 0;
    type208.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type208);
    uint32_t operand208 = operandIndex++;
    uint8_t operand208Value[] = {0, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand208, operand208Value, 4);

    ANeuralNetworksOperandType type209;
    type209.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type209.scale = 0;
    type209.zeroPoint = 0;
    type209.dimensionCount = 4;
    uint32_t type209Dims[] = {1, 9, 9, 17};
    type209.dimensions = type209Dims;
    ANeuralNetworksModel_addOperand(model, &type209);
    uint32_t operand209 = operandIndex++;

    ANeuralNetworksOperandType type210;
    type210.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type210.scale = 0;
    type210.zeroPoint = 0;
    type210.dimensionCount = 4;
    uint32_t type210Dims[] = {34, 1, 1, 1024};
    type210.dimensions = type210Dims;
    ANeuralNetworksModel_addOperand(model, &type210);
    uint32_t operand210 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand210, memory, 12853504, 139264);

    ANeuralNetworksOperandType type211;
    type211.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type211.scale = 0;
    type211.zeroPoint = 0;
    type211.dimensionCount = 1;
    uint32_t type211Dims[] = {34};
    type211.dimensions = type211Dims;
    ANeuralNetworksModel_addOperand(model, &type211);
    uint32_t operand211 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand211, memory, 12992768, 136);

    ANeuralNetworksOperandType type212;
    type212.type = ANEURALNETWORKS_INT32;
    type212.scale = 0;
    type212.zeroPoint = 0;
    type212.dimensionCount = 0;
    type212.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type212);
    uint32_t operand212 = operandIndex++;
    uint8_t operand212Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand212, operand212Value, 4);

    ANeuralNetworksOperandType type213;
    type213.type = ANEURALNETWORKS_INT32;
    type213.scale = 0;
    type213.zeroPoint = 0;
    type213.dimensionCount = 0;
    type213.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type213);
    uint32_t operand213 = operandIndex++;
    uint8_t operand213Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand213, operand213Value, 4);

    ANeuralNetworksOperandType type214;
    type214.type = ANEURALNETWORKS_INT32;
    type214.scale = 0;
    type214.zeroPoint = 0;
    type214.dimensionCount = 0;
    type214.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type214);
    uint32_t operand214 = operandIndex++;
    uint8_t operand214Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand214, operand214Value, 4);

    ANeuralNetworksOperandType type215;
    type215.type = ANEURALNETWORKS_INT32;
    type215.scale = 0;
    type215.zeroPoint = 0;
    type215.dimensionCount = 0;
    type215.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type215);
    uint32_t operand215 = operandIndex++;
    uint8_t operand215Value[] = {0, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand215, operand215Value, 4);

    ANeuralNetworksOperandType type216;
    type216.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type216.scale = 0;
    type216.zeroPoint = 0;
    type216.dimensionCount = 4;
    uint32_t type216Dims[] = {1, 9, 9, 34};
    type216.dimensions = type216Dims;
    ANeuralNetworksModel_addOperand(model, &type216);
    uint32_t operand216 = operandIndex++;

    ANeuralNetworksOperandType type217;
    type217.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type217.scale = 0;
    type217.zeroPoint = 0;
    type217.dimensionCount = 4;
    uint32_t type217Dims[] = {32, 1, 1, 1024};
    type217.dimensions = type217Dims;
    ANeuralNetworksModel_addOperand(model, &type217);
    uint32_t operand217 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand217, memory, 12992904, 131072);

    ANeuralNetworksOperandType type218;
    type218.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type218.scale = 0;
    type218.zeroPoint = 0;
    type218.dimensionCount = 1;
    uint32_t type218Dims[] = {32};
    type218.dimensions = type218Dims;
    ANeuralNetworksModel_addOperand(model, &type218);
    uint32_t operand218 = operandIndex++;
    uint8_t operand218Value[] = {
            145, 210, 23,  62,  24,  100, 58,  188, 5,   19,  26,  62,  104, 16,  56,  188,
            93,  50,  219, 190, 19,  49,  47,  191, 6,   118, 133, 190, 137, 192, 197, 191,
            150, 180, 86,  191, 162, 180, 130, 191, 126, 244, 220, 190, 12,  67,  50,  191,
            17,  59,  137, 190, 142, 247, 198, 191, 120, 48,  89,  191, 16,  69,  130, 191,
            115, 61,  117, 189, 224, 173, 25,  190, 102, 84,  218, 61,  92,  9,   45,  62,
            3,   39,  212, 190, 85,  156, 118, 190, 43,  241, 145, 61,  22,  236, 69,  61,
            129, 18,  107, 189, 95,  231, 189, 60,  46,  146, 221, 62,  234, 85,  142, 62,
            219, 242, 212, 188, 112, 146, 89,  187, 251, 8,   138, 61,  41,  140, 36,  188};
    ANeuralNetworksModel_setOperandValue(model, operand218, operand218Value, 128);

    ANeuralNetworksOperandType type219;
    type219.type = ANEURALNETWORKS_INT32;
    type219.scale = 0;
    type219.zeroPoint = 0;
    type219.dimensionCount = 0;
    type219.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type219);
    uint32_t operand219 = operandIndex++;
    uint8_t operand219Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand219, operand219Value, 4);

    ANeuralNetworksOperandType type220;
    type220.type = ANEURALNETWORKS_INT32;
    type220.scale = 0;
    type220.zeroPoint = 0;
    type220.dimensionCount = 0;
    type220.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type220);
    uint32_t operand220 = operandIndex++;
    uint8_t operand220Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand220, operand220Value, 4);

    ANeuralNetworksOperandType type221;
    type221.type = ANEURALNETWORKS_INT32;
    type221.scale = 0;
    type221.zeroPoint = 0;
    type221.dimensionCount = 0;
    type221.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type221);
    uint32_t operand221 = operandIndex++;
    uint8_t operand221Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand221, operand221Value, 4);

    ANeuralNetworksOperandType type222;
    type222.type = ANEURALNETWORKS_INT32;
    type222.scale = 0;
    type222.zeroPoint = 0;
    type222.dimensionCount = 0;
    type222.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type222);
    uint32_t operand222 = operandIndex++;
    uint8_t operand222Value[] = {0, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand222, operand222Value, 4);

    ANeuralNetworksOperandType type223;
    type223.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type223.scale = 0;
    type223.zeroPoint = 0;
    type223.dimensionCount = 4;
    uint32_t type223Dims[] = {1, 9, 9, 32};
    type223.dimensions = type223Dims;
    ANeuralNetworksModel_addOperand(model, &type223);
    uint32_t operand223 = operandIndex++;

    ANeuralNetworksOperandType type224;
    type224.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type224.scale = 0;
    type224.zeroPoint = 0;
    type224.dimensionCount = 4;
    uint32_t type224Dims[] = {32, 1, 1, 1024};
    type224.dimensions = type224Dims;
    ANeuralNetworksModel_addOperand(model, &type224);
    uint32_t operand224 = operandIndex++;
    ANeuralNetworksModel_setOperandValueFromMemory(model, operand224, memory, 13123976, 131072);

    ANeuralNetworksOperandType type225;
    type225.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type225.scale = 0;
    type225.zeroPoint = 0;
    type225.dimensionCount = 1;
    uint32_t type225Dims[] = {32};
    type225.dimensions = type225Dims;
    ANeuralNetworksModel_addOperand(model, &type225);
    uint32_t operand225 = operandIndex++;
    uint8_t operand225Value[] = {
            198, 178, 166, 189, 81,  255, 29,  61,  141, 35,  147, 189, 249, 191, 63,  61,
            104, 238, 151, 62,  178, 201, 245, 62,  20,  122, 183, 61,  217, 63,  94,  63,
            132, 126, 13,  63,  175, 147, 71,  63,  207, 193, 154, 62,  218, 111, 250, 62,
            44,  218, 168, 61,  59,  232, 92,  63,  148, 241, 9,   63,  158, 13,  73,  63,
            45,  60,  169, 61,  213, 176, 243, 61,  165, 241, 115, 189, 133, 85,  207, 189,
            161, 67,  24,  62,  200, 50,  170, 61,  9,   238, 5,   190, 177, 160, 251, 188,
            33,  56,  251, 60,  105, 223, 97,  189, 255, 115, 61,  190, 80,  232, 87,  189,
            47,  244, 254, 61,  212, 125, 131, 61,  53,  203, 81,  188, 118, 174, 24,  61};
    ANeuralNetworksModel_setOperandValue(model, operand225, operand225Value, 128);

    ANeuralNetworksOperandType type226;
    type226.type = ANEURALNETWORKS_INT32;
    type226.scale = 0;
    type226.zeroPoint = 0;
    type226.dimensionCount = 0;
    type226.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type226);
    uint32_t operand226 = operandIndex++;
    uint8_t operand226Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand226, operand226Value, 4);

    ANeuralNetworksOperandType type227;
    type227.type = ANEURALNETWORKS_INT32;
    type227.scale = 0;
    type227.zeroPoint = 0;
    type227.dimensionCount = 0;
    type227.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type227);
    uint32_t operand227 = operandIndex++;
    uint8_t operand227Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand227, operand227Value, 4);

    ANeuralNetworksOperandType type228;
    type228.type = ANEURALNETWORKS_INT32;
    type228.scale = 0;
    type228.zeroPoint = 0;
    type228.dimensionCount = 0;
    type228.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type228);
    uint32_t operand228 = operandIndex++;
    uint8_t operand228Value[] = {1, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand228, operand228Value, 4);

    ANeuralNetworksOperandType type229;
    type229.type = ANEURALNETWORKS_INT32;
    type229.scale = 0;
    type229.zeroPoint = 0;
    type229.dimensionCount = 0;
    type229.dimensions = nullptr;
    ANeuralNetworksModel_addOperand(model, &type229);
    uint32_t operand229 = operandIndex++;
    uint8_t operand229Value[] = {0, 0, 0, 0};
    ANeuralNetworksModel_setOperandValue(model, operand229, operand229Value, 4);

    ANeuralNetworksOperandType type230;
    type230.type = ANEURALNETWORKS_TENSOR_FLOAT32;
    type230.scale = 0;
    type230.zeroPoint = 0;
    type230.dimensionCount = 4;
    uint32_t type230Dims[] = {1, 9, 9, 32};
    type230.dimensions = type230Dims;
    ANeuralNetworksModel_addOperand(model, &type230);
    uint32_t operand230 = operandIndex++;

    // Operations
    uint32_t operation0InputIndexes[] = {0, 1, 2, 3, 4, 5, 6};
    uint32_t operation0OutputIndexes[] = {7};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation0InputIndexes, 1,
                                      operation0OutputIndexes);

    uint32_t operation1InputIndexes[] = {7, 8, 9, 10, 11, 12, 13, 14};
    uint32_t operation1OutputIndexes[] = {15};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation1InputIndexes, 1, operation1OutputIndexes);

    uint32_t operation2InputIndexes[] = {15, 16, 17, 18, 19, 20, 21};
    uint32_t operation2OutputIndexes[] = {22};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation2InputIndexes, 1,
                                      operation2OutputIndexes);

    uint32_t operation3InputIndexes[] = {22, 23, 24, 25, 26, 27, 28, 29};
    uint32_t operation3OutputIndexes[] = {30};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation3InputIndexes, 1, operation3OutputIndexes);

    uint32_t operation4InputIndexes[] = {30, 31, 32, 33, 34, 35, 36};
    uint32_t operation4OutputIndexes[] = {37};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation4InputIndexes, 1,
                                      operation4OutputIndexes);

    uint32_t operation5InputIndexes[] = {37, 38, 39, 40, 41, 42, 43, 44};
    uint32_t operation5OutputIndexes[] = {45};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation5InputIndexes, 1, operation5OutputIndexes);

    uint32_t operation6InputIndexes[] = {45, 46, 47, 48, 49, 50, 51};
    uint32_t operation6OutputIndexes[] = {52};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation6InputIndexes, 1,
                                      operation6OutputIndexes);

    uint32_t operation7InputIndexes[] = {52, 53, 54, 55, 56, 57, 58, 59};
    uint32_t operation7OutputIndexes[] = {60};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation7InputIndexes, 1, operation7OutputIndexes);

    uint32_t operation8InputIndexes[] = {60, 61, 62, 63, 64, 65, 66};
    uint32_t operation8OutputIndexes[] = {67};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation8InputIndexes, 1,
                                      operation8OutputIndexes);

    uint32_t operation9InputIndexes[] = {67, 68, 69, 70, 71, 72, 73, 74};
    uint32_t operation9OutputIndexes[] = {75};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation9InputIndexes, 1, operation9OutputIndexes);

    uint32_t operation10InputIndexes[] = {75, 76, 77, 78, 79, 80, 81};
    uint32_t operation10OutputIndexes[] = {82};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation10InputIndexes, 1,
                                      operation10OutputIndexes);

    uint32_t operation11InputIndexes[] = {82, 83, 84, 85, 86, 87, 88, 89};
    uint32_t operation11OutputIndexes[] = {90};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation11InputIndexes, 1, operation11OutputIndexes);

    uint32_t operation12InputIndexes[] = {90, 91, 92, 93, 94, 95, 96};
    uint32_t operation12OutputIndexes[] = {97};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation12InputIndexes, 1,
                                      operation12OutputIndexes);

    uint32_t operation13InputIndexes[] = {97, 98, 99, 100, 101, 102, 103, 104};
    uint32_t operation13OutputIndexes[] = {105};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation13InputIndexes, 1, operation13OutputIndexes);

    uint32_t operation14InputIndexes[] = {105, 106, 107, 108, 109, 110, 111};
    uint32_t operation14OutputIndexes[] = {112};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation14InputIndexes, 1,
                                      operation14OutputIndexes);

    uint32_t operation15InputIndexes[] = {112, 113, 114, 115, 116, 117, 118, 119};
    uint32_t operation15OutputIndexes[] = {120};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation15InputIndexes, 1, operation15OutputIndexes);

    uint32_t operation16InputIndexes[] = {120, 121, 122, 123, 124, 125, 126};
    uint32_t operation16OutputIndexes[] = {127};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation16InputIndexes, 1,
                                      operation16OutputIndexes);

    uint32_t operation17InputIndexes[] = {127, 128, 129, 130, 131, 132, 133, 134};
    uint32_t operation17OutputIndexes[] = {135};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation17InputIndexes, 1, operation17OutputIndexes);

    uint32_t operation18InputIndexes[] = {135, 136, 137, 138, 139, 140, 141};
    uint32_t operation18OutputIndexes[] = {142};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation18InputIndexes, 1,
                                      operation18OutputIndexes);

    uint32_t operation19InputIndexes[] = {142, 143, 144, 145, 146, 147, 148, 149};
    uint32_t operation19OutputIndexes[] = {150};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation19InputIndexes, 1, operation19OutputIndexes);

    uint32_t operation20InputIndexes[] = {150, 151, 152, 153, 154, 155, 156};
    uint32_t operation20OutputIndexes[] = {157};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation20InputIndexes, 1,
                                      operation20OutputIndexes);

    uint32_t operation21InputIndexes[] = {157, 158, 159, 160, 161, 162, 163, 164};
    uint32_t operation21OutputIndexes[] = {165};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation21InputIndexes, 1, operation21OutputIndexes);

    uint32_t operation22InputIndexes[] = {165, 166, 167, 168, 169, 170, 171};
    uint32_t operation22OutputIndexes[] = {172};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation22InputIndexes, 1,
                                      operation22OutputIndexes);

    uint32_t operation23InputIndexes[] = {172, 173, 174, 175, 176, 177, 178, 179};
    uint32_t operation23OutputIndexes[] = {180};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation23InputIndexes, 1, operation23OutputIndexes);

    uint32_t operation24InputIndexes[] = {180, 181, 182, 183, 184, 185, 186};
    uint32_t operation24OutputIndexes[] = {187};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation24InputIndexes, 1,
                                      operation24OutputIndexes);

    uint32_t operation25InputIndexes[] = {187, 188, 189, 190, 191, 192, 193, 194};
    uint32_t operation25OutputIndexes[] = {195};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_DEPTHWISE_CONV_2D, 8,
                                      operation25InputIndexes, 1, operation25OutputIndexes);

    uint32_t operation26InputIndexes[] = {195, 196, 197, 198, 199, 200, 201};
    uint32_t operation26OutputIndexes[] = {202};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation26InputIndexes, 1,
                                      operation26OutputIndexes);

    uint32_t operation27InputIndexes[] = {202, 224, 225, 226, 227, 228, 229};
    uint32_t operation27OutputIndexes[] = {230};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation27InputIndexes, 1,
                                      operation27OutputIndexes);

    uint32_t operation28InputIndexes[] = {202, 217, 218, 219, 220, 221, 222};
    uint32_t operation28OutputIndexes[] = {223};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation28InputIndexes, 1,
                                      operation28OutputIndexes);

    uint32_t operation29InputIndexes[] = {202, 210, 211, 212, 213, 214, 215};
    uint32_t operation29OutputIndexes[] = {216};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation29InputIndexes, 1,
                                      operation29OutputIndexes);

    uint32_t operation30InputIndexes[] = {202, 203, 204, 205, 206, 207, 208};
    uint32_t operation30OutputIndexes[] = {209};
    ANeuralNetworksModel_addOperation(model, ANEURALNETWORKS_CONV_2D, 7, operation30InputIndexes, 1,
                                      operation30OutputIndexes);

    // Input and output indexes
    uint32_t modelInputIndexes[] = {0};
    uint32_t modelOutputIndexes[] = {223, 230, 209, 216};
    ANeuralNetworksModel_identifyInputsAndOutputs(model, 1, modelInputIndexes, 4,
                                                  modelOutputIndexes);
}

}  // namespace pose_estimation
