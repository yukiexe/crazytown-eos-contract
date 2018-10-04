echo "Current working directory -" $1
cd $1

eosio-cpp -g $2.abi $2.cpp