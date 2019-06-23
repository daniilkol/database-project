./client.out < init_commands.txt
for ((i = 0; i < 20; i++))
    do 
	./client.out < client_commands.txt > /tmp/res$i &
    done
wait
./client.out < finish_commands.txt
