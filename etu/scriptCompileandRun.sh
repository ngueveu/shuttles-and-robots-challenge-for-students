
if [ -z $1 ]
then
echo "input argument is missing ! please specify the name of prefix of your files"
exit
fi

if [ ! -f $1_main_commande.cpp ]
then
  echo the file $1_main_commande.cpp does not exists
  exit
fi

cp $1_main_commande.cpp ../celluleflexible/ros_ws/src/commande/src/main_commande.cpp
cd ../celluleflexible/ros_ws
catkin_make
source devel/setup.bash
cd ../../etu
../celluleflexible/./launch.sh
cp ../celluleflexible/log.txt $1_log.txt

#python3 ../celluleflexible/checker.py $1_ProdConfig.config $1_log.txt  



