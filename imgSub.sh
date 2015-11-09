images=$1
if [ -z $images ];then
  echo "please specify image number"
  exit
fi
for((i=1;i<=$images;i++)){
  cd "Fe$i"
  modRunvasp.sh
  cd ..
}
