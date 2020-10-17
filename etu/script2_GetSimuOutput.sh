
if [ -z $1 ]
then
echo "the first input argument is missing ! please specify the name of prefix of your main command file (without .main_commande.cpp)"
exit
fi

if [ ! -f ../celluleflexible/log.txt ]
then
  echo the log file does not exists. An error probably occured during the simulation !!!
  exit
fi

if [ -z $2 ]
then
echo "the second input argument is missing ! please specify the name of product configuration file (without .prodconfig.h)"
exit
fi

cp ../celluleflexible/log.txt output/$1_$2.log.txt

cp ../celluleflexible/forTER/XXX_XXX_Doxyfile_config output/$1.doxyfileconfig.txt
sed -i 's/XXX_XXX/$1/g' output/$1.doxyfileconfig.txt





