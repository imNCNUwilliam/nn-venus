# Rework Neural Networks with low bit weights on Venus
This is my term project of CA2024, [NCKU CSIE](https://www.csie.ncku.edu.tw/), Tainan, Taiwan. 
Welcome to refer my [development notes](https://hackmd.io/@sysprog/Bky8cMDBkx#Open-source-project-to-leverage-Quantization) and give me feedbacks!

## INTRODUCTION
[Neural Networks (NN)](https://en.wikipedia.org/wiki/Neural_network_(machine_learning)) often give people the impression which consumes lots of memory and needs powerful processors. However, how much memory does it consume? Also, how about the processors capable of running the NN model? In this project, we focus on the fully-connected neural networks (FCNN) to discuss the following topics and seek cost-effective solutions to run Neural Networks (NN). 
* Memory consumptions for a given NN model 
* Model compression: Quantization approach 
* Open-source project to leverage Quantization 
* RV32IM implementation for Quantization approach

## RV32IM implementation of Quantization approach

### Binary files conversion
[BitNetMCU](https://github.com/cpldcpu/BitNetMCU) provides the sample dataset and model weights in C source code, so we need a tool program to convert them into binary files.

* BitNetMCU uses [MNIST dataset](https://en.wikipedia.org/wiki/MNIST_database) as the handwritten recognition dataset, and resizes images from 28x28 pixel to 16x16 pixel format. Each image is presented as 256 8-bit grayscales, and stored as 256 data. We convert each data into a 4-byte memory word, so the dataset will occupy 1024 bytes.
* In addition, BitNetMCU adopts Quantization approach and uses 4-byte data structure as the weight chunk. The `bitperweight` configuration determines how many weights are accommodated within a 4-byte weight chunk, and it cause that weights of the same NN model structure occupy different numbers of weight chunks according to the `bitperweight` configuration.

    | bitperweight              | 8    | 4    | 2    | 1    |
    | ------------------------- | ---- | ---- | ---- | ---- |
    | weights in a weight chunk | 4    | 8    | 16   | 32   |

Thus, the conversion aims to deal with different data types and different numbers of data, and they are specified as the arguments for conversion.

Dataset is prepared in "BitNetMCU_MNIST_test_data.h". Sets of model weights stored in "BitNetMCU_model.h" is generated from [Step 2: Quantization of model weights](https://hackmd.io/@sysprog/Bky8cMDBkx#Open-source-project-to-leverage-Quantization).

```
$ git clone https://github.com/imNCNUwilliam/nn-venus && cd BitNetMCU
$ cd BitNetMCU && make check
// generate the input and the four set of model weights as binary files
```

:::warning 
Hyperparameters in model training could generate certain "BitNetMCU_model.h" for conversion. However, [current BitNetMCU](https://github.com/cpldcpu/BitNetMCU/commit/986b66143bb0bfb181c28e0aa229615bbb1c8145) only support NN model "FCMNIST" for inference tests. Besides, during exporting quantized model weights, BitNetMCU.py causes some problems, and needs some patches. Please patch the one in the patch/ to fix the problem.
:::

### RV32IM inference program
We base on [UCB CS61C Fall 2024 Project: Classify](https://cs61c.org/fa24/projects/proj2/) to develop the RV32IM inference program. It is a test and development framework for RV32IM programs on [UCB CS61C Venus (RISV-V Simulator)](https://venus.cs61c.org/). After importing from the [Github repository](https://github.com/imNCNUwilliam/classify-rv32i), this project needs to finish these two major functions, i.e., Matrix Multiplication, and ReLU Normalization, have to be finished, together with the Quantization.

