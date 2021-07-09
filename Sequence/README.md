Android Neural Networks API Sample: Sequence
======

Introduction
----------

Android Neural Networks API (NN API) Sample demonstrates basic usages of NN API with a sequence model that consists of two operations: one addition and one multiplication. This graph is used for computing a single step of accumulating a geometric progression.

```
  sumIn ---+
           +--- ADD ---> sumOut
stateIn ---+
           +--- MUL ---> stateOut
  ratio ---+
```

The ratio is a constant tensor, defined in the model. It represents the weights that would have been learned during a training process. The sumIn and stateIn are input tensors. Their values will be provided when we execute the model. These values can change from execution to execution. To compute the sum of a geometric progression, the graph will be executed multiple times with inputs and outputs chained together.

```
                +----------+   +----------+         +----------+
  initialSum -->| Simple   |-->| Simple   |-->   -->| Simple   |--> sumOut
                | Sequence |   | Sequence |   ...   | Sequence |
initialState -->| Model    |-->| Model    |-->   -->| Model    |--> stateOut
                +----------+   +----------+         +----------+
```

Pre-requisites
----------

- Android Studio 4.0+
- NDK r22+
- Android API 30+

Getting Started
----------

- Download Android Studio.
- Launch Android Studio.
- Open the sample directory.
- Click Tools/Android/Sync Project with Gradle Files.
- Click Run/Run 'app'.

Screenshots
----------
<img src="screenshot.png" width="480">

Support
----------

We highly recommend to use [Stack Overflow](http://stackoverflow.com/questions/tagged/android) to get help from the Andorid community.

If you've found an error in this sample, please file an issue: https://github.com/android/neural-networks-samples

Patches are encouraged, and may be submitted by forking this project and submitting a pull request through GitHub.
