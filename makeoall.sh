cd vendor/hisilicon/prebuilts; ls | grep -v Android.mk |xargs rm -rf;cd ../../..;
mkdir -p ../out/compiling;
unzip -o $1 -d $(pwd)/vendor/hisilicon/prebuilts;
source build/envsetup.sh;lunch Hi3796CV300_SEC-eng;
mkdir ../out/$2;
rm -fr out;
./device/hisilicon/build/build.sh -o |tee /home/zhy123/out/$2/build_$2.log;
cp out/target/product/Hi3796CV300/*.zip ../out/$2;
cp -r out/target/product/Hi3796CV300/symbols ../out/$2;
cp -r out/target/product/Hi3796CV300/Emmc ../out/$2;
rm -fr ../out/compiling;