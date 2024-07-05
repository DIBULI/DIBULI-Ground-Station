#!/bin/bash
script_path="$(cd "$(dirname "$0")" && pwd)"
project_root_dir="$(dirname "$script_path")"

mkdir -p ${project_root_dir}/third_party

# add imgui support
cd ${project_root_dir}/third_party
if [ ! -d "imgui" ]; then
  git clone https://github.com/ocornut/imgui.git
  cd imgui
  git checkout 3369cbd2776d7567ac198b1a3017a4fa2d547cc3
fi

# compile eigen
cd ${project_root_dir}/third_party
if [ ! -d "eigen" ]; then
  git clone https://gitlab.com/libeigen/eigen.git
  cd eigen
  git checkout 3dc3a0ea2d0773af4c0ffd7bbcb21c608e28fcef
fi

cd ${project_root_dir}/third_party/eigen
rm -rf build install
mkdir -p build && mkdir -p install && cd build
cmake -DCMAKE_INSTALL_PREFIX=${project_root_dir}/third_party/eigen/install -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j 4
cmake --install .


# add glm
cd ${project_root_dir}/third_party
if [ ! -d "glm" ]; then
  git clone git@github.com:g-truc/glm.git
  cd glm
  git fetch --tags
  git checkout -b 1.1.0  tags/1.1.0
fi

cd ${project_root_dir}/third_party/glm
rm -rf build install
mkdir -p build && mkdir -p install && cd build
cmake -DCMAKE_INSTALL_PREFIX=${project_root_dir}/third_party/glm/install -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .


# add glfw
cd ${project_root_dir}/third_party
if [ ! -d "glfw" ]; then
  git clone https://github.com/glfw/glfw.git
  cd glfw
  git checkout bf945f1213728a98f7647380616f9cff9f6b3611
fi

cd ${project_root_dir}/third_party/glfw
rm -rf build install
mkdir -p build && mkdir -p install && cd build
cmake -DCMAKE_INSTALL_PREFIX=${project_root_dir}/third_party/glfw/install -DGLFW_BUILD_WIN32=$GLFW_BUILD_WIN32 -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .

