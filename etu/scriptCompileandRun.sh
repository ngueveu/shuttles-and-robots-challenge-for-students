
if [ -z $1 ]
then
echo "the first input argument is missing ! please specify the name of prefix of your files"
exit
fi

if [ ! -f $1_main_commande.cpp ]
then
  echo the file $1_main_commande.cpp does not exists
  exit
fi

if [ -z $2 ]
then
echo "the second input argument is missing ! please specify the name of product configuration file (without .h)"
exit
fi

if [ ! -f dataprodconfig/$2_prodconfig.h ]
then
  echo the file $2_prodconfig.h does not exists in folder dataprodconfig
  exit
fi

cp $1_main_commande.cpp ../celluleflexible/ros_ws/src/commande/src/main_commande.cpp
cp dataprodconfig/$2_prodconfig.h ../celluleflexible/ros_ws/src/commande/src/prodconfig/productconfig.h
cd ../celluleflexible/ros_ws
catkin_make
source devel/setup.bash
cd ../../etu
../celluleflexible/./launch.sh
cp ../celluleflexible/log.txt output/$1_$2_log.txt

#python3 ../celluleflexible/checker.py $1_ProdConfig.config $1_log.txt  



