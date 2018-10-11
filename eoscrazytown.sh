cleos wallet unlock -n www --password PW5JDQJvz73w73eNEtthCRhLn9gieFki5uiZu1JZdjwMftVA3f4cG

cleos -u http://api-direct.eosasia.one set contract crazytown.bp eoscrazytown -p crazytown.bp@active

cleos -u http://api-direct.eosasia.one push action crazytown.bp init '["0196d5b5d9ec1bc78ba927d2db2cb327d836f002601c77bd8c3f144a07ddc737"]' -p crazytown.bp@active

cleos -u http://api-direct.eosasia.one push action crazytown.bp reveal '["0196d5b5d9ec1bc78ba927d2db2cb327d836f002601c77bd8c3f144a07ddc737","0196d5b5d9ec1bc78ba927d2db2cb327d836f002601c77bd8c3f144a07ddc737"]' -p crazytown.bp@active
 
#cleos -u http://api-direct.eosasia.one push action crazytown.bp setwhitelist '["PXL", "dacincubator"]' -p crazytown.bp@active

#cleos -u http://api-direct.eosasia.one push action crazytown.bp rmwhitelist '["PXL"]' -p crazytown.bp@active

#cleos -u http://api-direct.eosasia.one push action eosio.token transfer '[ "minakokojima", "crazytown.bp", "0.0010 EOS", "0.0010 PXL" ]' -p minakokojima@active

#cleos -u http://api-direct.eosasia.one push action dacincubator transfer '[ "eosotcbackup", "crazytown.bp", "0.0010 PXL", "0.0010 EOS" ]' -p eosotcbackup@active

#cleos -u http://api-direct.eosasia.one push action eosio.token transfer '[ "minakokojima", "crazytown.bp", "0.0100 EOS", "0.0100 PUB" ]' -p minakokojima@active

cleos -u http://api-direct.eosasia.one get table crazytown.bp crazytown.bp global

#cleos -u http://api-direct.eosasia.one get table eosotcbackup dacincubator order
#cleos -u http://api-direct.eosasia.one get table eosotcbackup youyouyouyou order
#cleos -u http://api-direct.eosasia.one get table eosotcbackup eosio.token order
#cleos -u http://api-direct.eosasia.one get table -l 65536 eosotcbackup eosio.token order


# cleos -u https://api-kylin.eosasia.one push action pomelodex111 test '[]' -p minakokojima@active


# cleos -u https://api-kylin.eosasia.one set contract pomelodex111 pomelo -p pomelodex111@active
# cleos -u https://api-kylin.eosasia.one push action eosio.token transfer '[ "minakokojima", "pomelodex111", "1.0000 EOS", "buy,happyeosslot,HPY,2.0000" ]' -p minakokojima@active
# cleos -u https://api-kylin.eosasia.one set contract pomelodex111 pomelo -p pomelodex111@active
# cleos -u https://api-kylin.eosasia.one push action eosio.token transfer '[ "minakokojima", "happyeosslot", "1.0000 EOS", "buy" ]' -p minakokojima@active

# cleos -u https://api-kylin.eosasia.one push action eosio.token transfer '[ "minakokojima", "pomelodex111", "1.0000 EOS", "buy,happyeosslot,HPY,2.0000" ]' -p minakokojima@active
# /usr/local/eosio/bin/eosiocpp -g pomelo/pomelo.abi pomelo/pomelo.cpp
# cleos -u https://api-kylin.eosasia.one set contract pomelodex111 pomelo -p pomelodex111@active
# cleos -u https://api-kylin.eosasia.one system newaccount --stake-net '5.0000 EOS' --stake-cpu '1.0000 EOS' --buy-ram '20.0000 EOS' minakokojima pomelodex111 EOS5fY2dmpfXmzXN1DEJ6VfvhhCr55ZpE9MyTd9eX7Cw2EmzJcnFM EOS781pFN6TgUkuCDqmCdyxcp1cnJdtg3DYfajbAba6mvHvrFNuDy


# buy,happyeosslot,HPY,2.0000