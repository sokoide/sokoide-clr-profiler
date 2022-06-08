# README

## Prereq

* On Mac, please install clang by `brew install llvm`. clang comes with Xcode fails to build it.

## Build

```sh
# clone or download https://github.com/dotnet/runtime
# only /src/coreclr/pal/ and /src/coreclr/inc are necessary
make
```

## Run

```sh
export CORECLR_PROFILER={DB03B7FB-2425-4CBF-A375-D9A67A66C370}
export CORECLR_ENABLE_PROFILING=1
export CORECLR_PROFILER_PATH=/path/to/sokoide-profiler.so

# run any dotnet app here
```

## Links

* <https://assets.ctfassets.net/9n3x4rtjlya6/QDfdxN66cmy75OCOyts9G/b1b169083ecbb76ead56bca3cc3ecbea/100620_757082830_Pavel_Yosifovich_Writing_a_.NET_Core_cross_platform_profiler_in_an_hour.pdf>
* <https://github.com/microsoft/clr-samples/tree/master/ProfilingAPI>
