#!/bin/bash
script_path="$(cd "$(dirname "$0")" && pwd)"
project_root_dir="$(dirname "$script_path")"

mkdir -p ${project_root_dir}/third_party

# add imgui support
cd ${project_root_dir}/third_party
if [ ! -d "imgui" ]; then
  git clone https://github.com/ocornut/imgui.git
  cd imgui
  git checkout c71e4e8c7cb9b42b460bbaedfa4bc443f885b05b
fi

cd ${project_root_dir}/third_party
rm -rf implot
git clone git@github.com:epezent/implot.git
cd implot
cp implot* ../imgui

# add imgui plot support
cd ${project_root_dir}/third_party
if [ ! -d "implot" ]; then
  git clone git@github.com:epezent/implot.git
  cd implot
  git checkout f156599
  cp *.h ../imgui
  cp *.cpp ../imgui
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

# add cserialport
cd ${project_root_dir}/third_party
if [ ! -d "CSerialPort" ]; then
  git clone git@github.com:itas109/CSerialPort.git
  cd CSerialPort
  git checkout c3e49fe
fi

cd ${project_root_dir}/third_party/CSerialPort
rm -rf build install
mkdir -p build && mkdir -p install && cd build
cmake -DCMAKE_INSTALL_PREFIX=${project_root_dir}/third_party/CSerialPort/install -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cmake --install .



cd ${project_root_dir}/third_party
if [ ! -d "ImGuiFileDialog" ]; then
  git clone https://github.com/aiekick/ImGuiFileDialog.git
  cd ImGuiFileDialog
  git checkout f73e29fca08163fdcbd1e58fb7b67c7e56f5fa2e
  cp *.h ../imgui
  cp *.cpp ../imgui
fi

cd -



