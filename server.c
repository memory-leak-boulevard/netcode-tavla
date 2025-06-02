/*
    netcode reference implementation

    Copyright © 2017 - 2024, Mas Bandwidth LLC

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
           in the documentation and/or other materials provided with the distribution.

        3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived 
           from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
    USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "netcode.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>

int beyaz_kirik;
int siyah_kirik;
int beyaz_toplanmis;
int siyah_toplanmis;

enum tas_rengi{
		BOS,
		BEYAZ,
		SIYAH,
	};

struct sutun{
		enum tas_rengi renk;
		int tas_sayisi;
	};

void zar_at(int* zar1, int* zar2) {
	
	*zar1 = rand() % 6 + 1;
	*zar2 = rand() % 6 + 1;
	
}

int yerlestir(char column, int renk, struct sutun tahta[]) {
	int destination_pos = column - 'A';
	int destination_renk = tahta[destination_pos].renk;
		if (renk == destination_renk) {
		tahta[destination_pos].tas_sayisi++;
	}


	else if (destination_renk == BOS) {
		tahta[destination_pos].tas_sayisi++;
		tahta[destination_pos].renk = renk;

	}

	else {
		tahta[destination_pos].renk = renk;
		if (renk == SIYAH) {
			beyaz_kirik++;
		}
		else {
			siyah_kirik++;
		}
	}

	switch (renk) {
		case BEYAZ: beyaz_kirik--; break;
		case SIYAH: siyah_kirik--; break;
		default: printf("bir seyler cok yanlis gitti\n");  break;
	}


	return 0;
}

int topla(char column, struct sutun tahta[]) {
	int column_int = column - 'A';
	if (tahta[column_int].renk == BEYAZ) {
		beyaz_toplanmis++;
	}
	else  {
		siyah_toplanmis++;
	}
	tahta[column_int].tas_sayisi--;
	if (tahta[column_int].tas_sayisi==0) {
		tahta[column_int].renk = BOS;
	}
	return 0;
}


int hareket_ettir(char start, char destination, struct sutun tahta[]) {

	int start_pos = start - 'A';
	int destination_pos = destination - 'A';

	int start_renk = tahta[start_pos].renk;
	int destination_renk = tahta[destination_pos].renk;

	tahta[start_pos].tas_sayisi--;

	int eski_renk = tahta[start_pos].renk;

	if (tahta[start_pos].tas_sayisi == 0) {
		tahta[start_pos].renk = BOS;
	}

	if (start_renk == destination_renk) {
		tahta[destination_pos].tas_sayisi++;
	//	printf("start ve destination rengi ayni\n");
	}


	else if (destination_renk == BOS) {
		tahta[destination_pos].tas_sayisi++;
		tahta[destination_pos].renk = eski_renk;
	//	printf("destination bos\n");
	}

	else {
		tahta[destination_pos].renk = eski_renk;
		if (eski_renk == SIYAH) {
			beyaz_kirik++;
		}
		else {
			siyah_kirik++;
		}
	//	printf("start ve destination rengi farkli\n");
	}

	return 0;

}

void tahta_ciz(struct sutun tahta[]) {

	for(int i=12 ; i<24 ; i++) {

		printf(" %c ", i + 'A');

		if (i==17){ printf(" | ");}
	
	
	}
	

	printf("\n");

	for(int i=12 ; i<24 ; i++) {

		switch(tahta[i].renk) {
		case BEYAZ: printf("b%-2d", tahta[i].tas_sayisi ); break;
		case SIYAH: printf("s%-2d", tahta[i].tas_sayisi ); break;
			default: printf("   "); break;
		}

		if (i==17){ printf(" | ");}

	}

	printf("\n                   |\n");


	for(int i=11 ; i>-1 ; i--) {

		
		switch(tahta[i].renk) {
		case BEYAZ: printf("b%-2d", tahta[i].tas_sayisi ); break;
		case SIYAH: printf("s%-2d", tahta[i].tas_sayisi ); break;
			default: printf("   "); break;
		}

		if (i==6) {printf(" | ");}

	}

	printf("\n");

	for(int i=11 ; i>-1 ; i--) {

		 printf(" %c ", i + 'A');
		if (i==6){ printf(" | ");}

	}

	printf("\n");

	printf("beyaz kirik: %d, siyah kirik: %d\nbeyaz toplanmis: %d, siyah toplanmis: %d\n", beyaz_kirik, siyah_kirik,
			beyaz_toplanmis, siyah_toplanmis);


}
static volatile int quit = 0;

void interrupt_handler( int signal )
{
    (void) signal;
    quit = 1;
}

static uint8_t private_key[NETCODE_KEY_BYTES] = { 0x60, 0x6a, 0xbe, 0x6e, 0xc9, 0x19, 0x10, 0xea, 
                                                  0x9a, 0x65, 0x62, 0xf6, 0x6f, 0x2b, 0x30, 0xe4, 
                                                  0x43, 0x71, 0xd6, 0x2c, 0xd1, 0x99, 0x27, 0x26,
                                                  0x6b, 0x3c, 0x60, 0xf4, 0xb7, 0x15, 0xab, 0xa1 };

int main( int argc, char ** argv )
{
    (void) argc;
    (void) argv;

    if ( netcode_init() != NETCODE_OK )
    {
        printf( "error: failed to initialize netcode\n" );
        return 1;
    }

    netcode_log_level( NETCODE_LOG_LEVEL_INFO );

    double time = 0.0;
    double delta_time = 1.0 / 60.0;

    printf( "[server]\n" );

    #define TEST_PROTOCOL_ID 0x1122334455667788

    char * server_address = "127.0.0.1:40000";
    if ( argc == 2 )
        server_address = argv[1];

    struct netcode_server_config_t server_config;
    netcode_default_server_config( &server_config );
    server_config.protocol_id = TEST_PROTOCOL_ID;
    memcpy( &server_config.private_key, private_key, NETCODE_KEY_BYTES );

    struct netcode_server_t * server = netcode_server_create( server_address, &server_config, time );

    if ( !server )
    {
        printf( "error: failed to create server\n" );
        return 1;
    }

    netcode_server_start( server, NETCODE_MAX_CLIENTS );

    signal( SIGINT, interrupt_handler );

//    uint8_t packet_data[NETCODE_MAX_PACKET_SIZE];
//    int i;
//   for ( i = 0; i < NETCODE_MAX_PACKET_SIZE; ++i ){
//        packet_data[i] = (uint8_t) i;
//	}


	
	int beyaz_kirik=0;
	int siyah_kirik=0;
	int beyaz_toplanmis=0;
	int siyah_toplanmis=0;

	// unused variables error vermesin diye en kotu cozumu uyguladim
	
	assert(0 == beyaz_kirik + siyah_kirik + beyaz_toplanmis + siyah_toplanmis);

	struct sutun tahta[24] = { {SIYAH,2}, {}, {}, {}, {},  {BEYAZ,5}, 
		{}, {BEYAZ,3}, {}, {}, {}, {SIYAH,5},
		{BEYAZ,5}, {}, {}, {}, {SIYAH,3}, {},
		{SIYAH,5}, {}, {}, {}, {}, {BEYAZ,2},
	};

	tahta_ciz(tahta);	

	
	char start,destination;

	int tahta_cizme = 0;
	
	int zar1 = 0;
	int zar2 = 0;
    
    while ( !quit )
    {
        netcode_server_update( server, time );

       // if ( netcode_server_client_connected( server, 0 ) )
       // {
       //     netcode_server_send_packet( server, 0, packet_data, NETCODE_MAX_PACKET_SIZE );
       // }

        int client_index;
        for ( client_index = 0; client_index < NETCODE_MAX_CLIENTS; ++client_index )
        {
	    while ( 1 )             
            {
			int packet_bytes;
                uint64_t packet_sequence;
                void * packet = netcode_server_receive_packet( server, client_index, &packet_bytes, &packet_sequence );
                if ( !packet )           { break;}
                (void) packet_sequence;
                assert( packet_bytes == NETCODE_MAX_PACKET_SIZE );
		if (((uint8_t*)packet)[0] !=(uint8_t) 0)	{
			printf("Received packet: %c%c\n",((char*)packet)[0],
				((char*)packet)[1]);
			start = ((char*)packet)[0];
			destination = ((char*)packet)[1];
			switch (start) {
				case 't': topla(destination,tahta); break;
				case 'b': yerlestir(destination,BEYAZ,tahta); break;
				case 's': yerlestir(destination,SIYAH,tahta); break;	
				case 'r':{zar_at(&zar1,&zar2); tahta_cizme = 1;} break;
				default: hareket_ettir(start,destination, tahta); break;
		
			}

			if (!tahta_cizme) {
				tahta_ciz(tahta);
			}

			else {tahta_cizme = 0;
				((uint8_t*)packet)[0] = zar1;
				((uint8_t*)packet)[1] = zar2;
			}
			

		// now tell everyone else what this move is

			for(int k = 0; k< NETCODE_MAX_CLIENTS; k++) {
				 netcode_server_send_packet( server, k, packet, NETCODE_MAX_PACKET_SIZE );
		
			}
		}		
 
                netcode_server_free_packet( server, packet );

	    }    

	}

        netcode_sleep( delta_time );

        time += delta_time;
    }

    if ( quit )
    {
        printf( "\nshutting down\n" );
    }

    netcode_server_destroy( server );

    netcode_term();
    
    return 0;
}
