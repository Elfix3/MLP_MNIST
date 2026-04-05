# MLP Builder — C++ Neural Network Training Tool

A command-line application for building, training, evaluating, and saving **Multi-Layer Perceptrons (MLPs)** from scratch in C++, designed around the MNIST handwritten digit dataset.

---

## Features

- **Custom MLP architecture** — define the number of layers and neurons interactively
- **Forward & backward propagation** — implemented from scratch using a custom `Matrix` class
- **Activation functions** — ReLU (hidden layers) and Softmax (output layer)
- **Mini-batch gradient descent** — configurable batch size, learning rate, epochs, and shuffle seed
- **Cross-entropy loss** — with numerical stability (epsilon clamping)
- **Per-digit accuracy evaluation** — global accuracy and breakdown by digit class
- **Save & load networks** — custom binary format with magic number validation
- **Interactive terminal menu** — navigate all features through a dynamic menu system

---

## Project Structure

```
.
├── Matrix.cpp / Matrix.h          # Core matrix operations (add, mul, transpose, RELU, SOFTMAX...)
├── Layer.cpp / Layer.h            # Single neural network layer (forward, backward, update)
├── NeuralNetwork.cpp / .h         # Full MLP (forward pass, backprop, loss, save/load)
├── NnManager.cpp / .h             # Manages multiple networks + training config
├── MNISTReader.cpp / .h           # Reads and parses raw MNIST binary files
├── MenuNavigator.cpp / .h         # Dynamic terminal menu system
├── cmd_line_tools.cpp / .h        # Secure input helpers
└── saved_networks/                # Folder for saved network binary files
```

---

## Requirements

- **C++11** or later
- **MNIST dataset** binary files (see below)
- `g++` and `make`
- *(Optional)* `gnuplot` for MNIST image visualization

---

## MNIST Dataset Setup

Download the original MNIST binary files and place them in a `data/` folder at the project root:

```
data/
├── train-images.idx3-ubyte
├── train-labels.idx1-ubyte
├── t10k-images.idx3-ubyte
└── t10k-labels.idx1-ubyte
```

> **Do not rename the files.** The reader checks for exact filenames.

The dataset can be downloaded from: http://yann.lecun.com/exdb/mnist/

---

## Build

The project uses a Makefile. Expected layout:

```
.
├── main.cpp
├── includes/       # Header files (.h)
├── sources/        # Source files (.cpp)
├── obj/            # Compiled object files (auto-created)
└── makefile
```

```bash
make        # Build the project → produces ./program
make clean  # Remove object files and binary
```

The Makefile compiles with `-std=c++11 -Wall` and automatically picks up all `.cpp` files in `sources/`.

> On Windows, `MNISTReader.cpp` includes `<winsock2.h>`. Link with `-lws2_32` if needed.

---

## Usage

Run the compiled binary:

```bash
./program.exe
```

You will be greeted by an interactive terminal menu:

```
====MLP training program====
1. Create network
2. Import network
3. Training config
4. Compare
5. Network list
6. Back
```

### Create a Network

Select **Create network** and follow the prompts:
- Enter a unique integer **network ID**
- Enter the desired **number of layers** (max 10)
- Enter the **number of neurons** per hidden layer (max 1024 per layer)

The last layer is always set to **10 neurons with Softmax** (for digit classification 0–9). Input size is fixed at **784** (28×28 MNIST images).

### Configure Training

Select **Training config** to adjust:

| Parameter     | Default | Description                        |
|---------------|---------|------------------------------------|
| Learning rate | —       | Step size for gradient descent     |
| Epochs        | —       | Number of full passes over data    |
| Batch size    | —       | Mini-batch size                    |
| Shuffle       | —       | Shuffle training data each epoch   |
| Seed          | —       | Random seed for reproducibility    |

### Train a Network

Go to **Network list**, select your network by ID, then choose **Train**. Training progress is printed epoch by epoch with the average cross-entropy loss.

### Evaluate a Network

From the network's submenu:
- **Global accuracy** — runs inference on all 10,000 MNIST test images and prints the overall accuracy
- **Accuracy per digit** — prints correct/total and percentage for each digit class (0–9)

### Save & Load

- **Save** — prompts for a filename and writes the network to `saved_networks/<filename>` in a custom binary format
- **Import network** — lists files in `saved_networks/` and loads the selected one

#### Binary Format

Each saved file follows this structure:

```
[4 bytes] Magic number  (0x4E4E3030 = untrained, 0x4E4E3031 = trained)
[4 bytes] Network ID
[4 bytes] Number of layers
[4 bytes] Input size (784 for MNIST)

For each layer:
  [4 bytes] Layer index
  [4 bytes] Activation type (0 = RELU, 3 = SOFTMAX)
  [4 bytes] Input size
  [4 bytes] Output size
  [8 * input * output bytes] Weight matrix (doubles)
  [8 * output bytes]         Bias vector (doubles)
```

---

## Architecture

### Matrix

All tensor operations are implemented in a custom `Matrix` class backed by a raw `double*` heap array. Supported operations include matrix multiplication, element-wise product (`^`), transpose, row/column summation, broadcasting, ReLU, and Softmax.

### Layer

Each `Layer` stores weights `W`, biases `b`, and their gradients `dW`, `dB`. It performs:
- **Forward pass**: `Z = X·W + b`, then activation (ReLU or Softmax)
- **Backward pass**: computes `dZ`, `dW`, `dB`, and returns `dA_prev = dZ·Wᵀ`
- **Update**: `W -= dW * lr`, `b -= dB * lr`

### NeuralNetwork

Chains multiple `Layer` objects. Exposes `forward()`, `backward()`, and `update()`. Computes **cross-entropy loss** both per-sample and averaged over a batch.

### NnManager

Manages up to 10 networks (dynamically resizable). Handles training loops with mini-batch sampling, optional shuffling, and loss reporting. Stores a global `TrainingConfig` shared across all training runs.

---

## Notes & Limitations

- Input size is hardcoded to **784** (MNIST 28×28). Adapting to other datasets requires modifying `createNeuralNetwork()` in `MenuNavigator.cpp`.
- Only **ReLU** and **Softmax** activations are supported. No sigmoid or tanh.
- The `Compare` menu option is not yet implemented.
- The project currently uses `<winsock2.h>` in `MNISTReader.cpp` (for byte-order handling on Windows). For Linux/macOS, replace with `<arpa/inet.h>` or remove if not needed.
- Weights are initialized with small random values in `[-0.08, 0.08]` (normalized uniform distribution).

---

## License

This project is provided as-is for educational purposes.
