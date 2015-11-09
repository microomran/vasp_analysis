read -a elements
echo "Automatic"
echo "0"
echo "Monkhorst"
for i in ${elements[@]};
do
  echo -n "$i ";
done;
echo "";
echo "0 0 0"
