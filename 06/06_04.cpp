url="https://sourceforge.net/projects/boost/files/boost/1.85.0/boost_1_85_0.tar.gz/download"

wget -O boost_1_85_0.tar.gz $url

tar xvf boost_1_85_0.tar.gz

cd boost_1_85_0/

sudo apt update

sudo apt install autotools-dev build-essential libboost-all-dev libbz2-dev

./bootstrap.sh

options="toolset=gcc variant=release link=static runtime-link=static threading=multi"

sudo ./b2 $options -j8 install

./bootstrap.sh --with-python=python3

sudo ./b2 --with-python $options -j8 install