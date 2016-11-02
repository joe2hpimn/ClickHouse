# How to build ClickHouse

Build should work on Mac OS X 10.12.
With appropriate changes, build should work on any other OS X distribution.

## Maybe install Xcode XXX???

Use App Store.

## git? curl? ruby? XXX???

## Install Homebrew

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

## Install cmake, gcc

```
brew install cmake gcc
```

## Use GCC 6 for builds

```
export CC=gcc-6
export CXX=g++-6
```

## Install required libraries from packages

```
brew install icu4c mysql boost protobuf msgpack openssl unixodbc
```

## Disable MongoDB binding
```
export DISABLE_MONGODB=1
```

# Checkout ClickHouse sources

```
git clone git@github.com:yandex/ClickHouse.git
# or: git clone https://github.com/yandex/ClickHouse.git

cd ClickHouse
```

Note that master branch is not stable.
For stable version, switch to some release branch.

# Build ClickHouse

```
mkdir build
cd build
cmake ..
make -j $THREADS
cd ..
```
