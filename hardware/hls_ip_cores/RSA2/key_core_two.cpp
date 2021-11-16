#define INTERFACE_100G

#include "key_core_two.hpp"
#include "cstddef"


void key_core_two
/*(
		ap_uint<32> id,
		hls::stream<data> &in, 	//in AXI port
		hls::stream<data> &out, 	//out AXI port
		ap_uint<128> &final_key		//FIXME: putting on out port for now for testing
){*/
(
    short id,
    galapagos_interface * in,
    galapagos_interface  * out,
	ap_uint<16> &timeStep
){
	#pragma HLS_DATAFLOW
	#pragma HLS INTERFACE ap_ctrl_none port=return
	// #pragma HLS resource core=AXI4Stream variable = in
	// #pragma HLS DATA_PACK variable=in

	// #pragma HLS resource core=AXI4Stream variable = out
	// #pragma HLS DATA_PACK variable=out

	#pragma HLS INTERFACE axis register both port=out
	#pragma HLS INTERFACE axis register both port=in
#pragma HLS INTERFACE axis register both port=timeStep

	//Message handling logistics
	static bool trans_done = 0;
	static ap_uint<16> time_step = 0;
	static ap_uint<32> key_exchange_partner_dest = 0;
	const bool is_sender = (id == SENDER_ID);

	//Key exchange variables
	static ap_uint<128> secret = 0;
	static ap_uint<128> public_key = 0;
	static ap_uint<128> shared_secret = 0;

	static ap_uint<128> modulus = 0; //large prime number
	static ap_uint<128> base = 0; //smaller number

	timeStep.write(time_step);

	size_t size;
	short dest, src;

	if(trans_done)
		return;

	//Load in the destination (i.e. who we are generating a key with)
	if( (time_step == 0) && !in->empty() ){
    // if( (time_step == 0) ){
		//Retrieve data and add to the sum
		// key_exchange_partner_dest = in.read().value(31,0);

        galapagos_packet data; 
        data = in->read();
        src = data.id;

        if (src == 0) { 
            key_exchange_partner_dest = (ap_uint<32>)data.data;

            galapagos_packet dataT;
            dataT.dest = 0;
            dataT.id = 2;
            dataT.keep = KEEP_ALL;
            dataT.last = 1;
            dataT.data = 0x201;
            out->write(dataT);

            galapagos_packet dataT1;
            dataT1.dest = 0;
            dataT1.id = 2;
            dataT1.keep = KEEP_ALL;
            dataT1.last = 1;
            dataT1.data = key_exchange_partner_dest;
            out->write(dataT1);

            secret = 1 + id;
            time_step = 1;
        }

		//printf("\n timestep 0 \n");

        // char flag = 0;
        // while (flag == 0)
        // {
        //     T * read_data = (T *)in->packet_read(&size, &dest, &src);

        //     if (src == 0) {
        //         // key_exchange_partner_dest = read_data[0] & 0xffffffff;
		// 		key_exchange_partner_dest = (ap_uint<32>)read_data[0];
		// 		// key_exchange_partner_dest = (*read_data).value(31,0);
        //         //TODO = *read_data.value(31,0);

		// 		// //printf("\n read from timestep 0- key exchange parter: %x \n", (unsigned int)key_exchange_partner_dest);

        //         secret = 12 + id;
        //         time_step++;

		// 		// // send garbage package to the other rsa
		// 		// char garbage = 'g';
		// 		// out->packet_write((char *)&garbage, 1, 0, id);

        //         flag = 1;
        //     }
        // }
        
	}

	//Sender/receiver logic needs to be split for the first few steps
	// T * read_data = (T *)in->packet_read(&size, &dest, &src);
	else if(time_step <= 2){
	// if(time_step <= 2){
		if(is_sender){
			//Modulus  generation
			// if(time_step == 1){

			// 	//printf("\n timestep 1 from rsa 1 \n");

			// 	//Generate modulus and send it
			// 	modulus = 524287;
			// 	const data out_data = {key_exchange_partner_dest, modulus, 1, 0};
			// 	// out.write(out_data);

            //     // it should send to the other RSA kernel
            //     out->packet_write((char *)&out_data, 1, 2 /*rec RSA id*/, 1 /*send RSA id*/);

			// 	time_step++;
			// }

			// else if(time_step == 2){

			// 	//printf("\n timestep 2 from rsa 1 \n");

			// 	//Generate base and send it
			// 	base = 23;
			// 	const data out_data = {key_exchange_partner_dest, base, 1, 0};
			// 	// out.write(out_data);

            //     // it should send to the other RSA kernel
            //     out->packet_write((char *)&out_data, 1, 2 /*rec RSA id*/, 1 /*send RSA id*/);
				
			// 	time_step++;
			// }
		}

		else{
			if(!in->empty()){
				// data read_data = in.read();

                size_t size;
                short dest, src;

                galapagos_packet data; 
                data = in->read();
                src = data.id;

                if (src == 1) {
                    if(time_step == 1){
                        //modulus = (ap_uint<128>)(data.data >> 64);
                    	modulus = (ap_uint<128>)(data.data);

                        galapagos_packet dataT;
                        dataT.dest = 0;
                                    dataT.id = 2;
                                    dataT.keep = KEEP_ALL;
                                    dataT.last = 1;
                                    dataT.data = 0x202;
                                    out->write(dataT);
                                    galapagos_packet dataT1;
                                    dataT1.dest = 0;
                                    dataT1.id = 2;
                                    dataT1.keep = KEEP_ALL;
                                    dataT1.last = 1;
                                    dataT1.data = modulus;
                                    out->write(dataT1);

                        time_step++;
                    }
                    else if(time_step == 2){
                        //base = (ap_uint<128>)(data.data >> 64);
                        base = (ap_uint<128>)(data.data);

                        galapagos_packet dataT;
                                    dataT.dest = 0;
                                    dataT.id = 2;
                                    dataT.keep = KEEP_ALL;
                                    dataT.last = 1;
                                    dataT.data = 0x203;
                                    out->write(dataT);
                       galapagos_packet dataT1;
                                    dataT1.dest = 0;
                                    dataT1.id = 2;
                                    dataT1.keep = KEEP_ALL;
                                    dataT1.last = 1;
                                    dataT1.data = base;
                                    out->write(dataT1);

                        time_step++;
                    }
                }

                // char flag = 0;
                // while (flag == 0)
                // {
                //     T * read_data = (T *)in->packet_read(&size, &dest, &src);

                //     if (src == 1) { /* if it's from the sender kernel */

                //         //Load modulus
                //         if(time_step == 1){

				// 			//printf("\n timestep 1 from rsa 2 \n");

                //             // modulus = read_data.value;
                //             // modulus = (read_data[0] >> 32) & 0xffffffffffffffffffffffffffffffff;
				// 			// modulus = (read_data[0] >> 32).value(127,0);
				// 			modulus = (ap_uint<128>)(read_data[0] >> 64);

                //             //TODO .value(127,0)

                //             time_step++;

                //             // flag = 1;
                //         }

                //         //Load base		
                //         else if(time_step == 2){
				// 		// if(time_step == 2){

				// 			//printf("\n timestep 2 from rsa 2 \n");

                //             // base = read_data.value;
                //             // base = (read_data[0] >> 32).value(127,0);
				// 			base = (ap_uint<128>)(read_data[0] >> 64);

                //             time_step++;

                //             flag = 1;
                //         }

                //     }
                // }

				// //Load modulus
				// if(time_step == 1){
				// 	modulus = read_data.value;
				// 	time_step++;
				// }

				// //Load base		
				// else if(time_step == 2){
				// 	base = read_data.value;
				// 	time_step++;
				// }	
			
            
            }

			
		}
	}

	//Latter time steps - send/receive phase is done
	else{
		//Public key generation
		if(time_step == 3){

			//printf("\n timestep 3 from rsa %d\n", id);

			galapagos_packet data10;
						                data10.dest = 0;
						                data10.id = 2;
						                data10.keep = KEEP_ALL;
						                data10.last = 1;
						                data10.data = 0x233;
						out->write(data10);

			//public_key = pow(base, secret) % self.modulus
			#ifdef ENABLE_DEBUG_PRINT
			std::cout << "CORE generating pub key with:\n\tbase: " << base << "\n\tsecret: " << secret << "\n\tmod: " << modulus << std::endl;
			#endif

			big_mult_t big_mult = base;

			galapagos_packet data5;
			                data5.dest = 0;
			                data5.id = 2;
			                data5.keep = KEEP_ALL;
			                data5.last = 1;
			                data5.data = base;
			out->write(data5);
			galapagos_packet data6;
			                data6.dest = 0;
			                data6.id = 2;
			                data6.keep = KEEP_ALL;
			                data6.last = 1;
			                data6.data = modulus;
			out->write(data6);
			galapagos_packet data7;
			                data7.dest = 0;
			                data7.id = 2;
			                data7.keep = KEEP_ALL;
			                data7.last = 1;
			                data7.data = big_mult;
			out->write(data7);

			pub_key_loop:
			for(ap_uint<128> i = 1; i < secret; i++){
				//#pragma HLS loop_tripcount min=1 max=256 avg=128
    			//#pragma HLS pipeline
    			//#pragma HLS unroll factor = 1
				big_mult = big_mult * ((ap_uint<512>)(base));
			}
			//big_mult = (big_mult % modulus);

			/*while (big_mult >= modulus) {
				#pragma HLS pipeline
				big_mult = big_mult - modulus;
			}*/

			galapagos_packet data3;
			                data3.dest = 0;
			                data3.id = 2;
			                data3.keep = KEEP_ALL;
			                data3.last = 1;
			                data3.data = base;
			out->write(data3);
			galapagos_packet data8;
			                data8.dest = 0;
			                data8.id = 2;
			                data8.keep = KEEP_ALL;
			                data8.last = 1;
			                data8.data = modulus;
			out->write(data8);
			galapagos_packet data9;
			                data9.dest = 0;
			                data9.id = 2;
			                data9.keep = KEEP_ALL;
			                data9.last = 1;
			                data9.data = big_mult;
			out->write(data9);

			big_mult_t f1 = big_mult / modulus;
			big_mult_t f2 = f1 * modulus;
			big_mult = big_mult - f2;

			public_key = big_mult;

			galapagos_packet data1;
			                data1.dest = 0;
			                data1.id = 2;
			                data1.keep = KEEP_ALL;
			                data1.last = 1;
			                data1.data = f1;
			out->write(data1);
			galapagos_packet data2;
			                data2.dest = 0;
			                data2.id = 2;
			                data2.keep = KEEP_ALL;
			                data2.last = 1;
			                data2.data = f2;
			out->write(data2);
			galapagos_packet data4;
			                data4.dest = 0;
			                data4.id = 2;
			                data4.keep = KEEP_ALL;
			                data4.last = 1;
			                data4.data = big_mult;
			out->write(data4);


			#ifdef ENABLE_DEBUG_PRINT
			std::cout << "CORE generated pub key: " << public_key << std::endl;
			#endif
			const data out_data = {key_exchange_partner_dest, public_key, 1, 0};

            //TODO: is this really needed
			// out.write(out_data);
            if (id == 1) {
                // out->packet_write((char *)&out_data, 1, 2, 1);
                galapagos_packet data;
                data.dest = 2;
                data.id = 1;
                data.keep = KEEP_ALL;
                data.last = 1;
                data.data = out_data.value;
                out->write(data);
            }
            else if (id == 2) {
                // out->packet_write((char *)&out_data, 1, 1, 2);
                galapagos_packet data;
                data.dest = 1;
                data.id = 2;
                data.keep = KEEP_ALL;
                data.last = 1;
                data.data = out_data.value;
                out->write(data);
            }

			time_step++;
		}

		//Shared secret generation
		else if(time_step == 4 && !in->empty()){

			//printf("\n timestep 4 from rsa %d\n", id);

			// const ap_uint<128> other_public_key = in.read().value;
            ap_uint<128> other_public_key;
            size_t size;
            short dest, src;

            galapagos_packet data; 
            data = in->read();
            src = data.id;
            if (src == 1) {
            //other_public_key = (ap_uint<128>)(data.data >> 64);
            other_public_key = (ap_uint<128>)(data.data);

            galapagos_packet datapk;
            datapk.dest = 0;
            datapk.id = 2;
            datapk.keep = KEEP_ALL;
            datapk.last = 1;
            datapk.data = other_public_key;
            out->write(datapk);

            // char flag = 0;
            // while (flag == 0)
            // {
            //     T * read_data = (T *)in->packet_read(&size, &dest, &src);

            //     if (src == 1 && dest == 2) {
            //         // other_public_key = (read_data[0] >> 32).value(127,0);
			// 		other_public_key = (ap_uint<128>)(read_data[0] >> 64);
                    
            //         flag = 1;
            //     }
            //     if (src == 2 && dest == 1) {
            //         // other_public_key = (read_data[0] >> 32).value(127,0);
			// 		other_public_key = (ap_uint<128>)(read_data[0] >> 64);
                    
            //         flag = 1;
            //     }
            // }

			//shared_secret = pow(other_public_key, secret) % modulus
			#ifdef ENABLE_DEBUG_PRINT
			std::cout << "CORE generating shared secret with:\n\tother pub: " << other_public_key << "\n\tsecret: " << secret << "\n\tmod: " << modulus << std::endl;
			#endif
			big_mult_t big_mult = other_public_key;

			galapagos_packet data5;
			                data5.dest = 0;
			                data5.id = 2;
			                data5.keep = KEEP_ALL;
			                data5.last = 1;
			                data5.data = base;
			out->write(data5);
			galapagos_packet data6;
			                data6.dest = 0;
			                data6.id = 2;
			                data6.keep = KEEP_ALL;
			                data6.last = 1;
			                data6.data = modulus;
			out->write(data6);
			galapagos_packet data7;
			                data7.dest = 0;
			                data7.id = 2;
			                data7.keep = KEEP_ALL;
			                data7.last = 1;
			                data7.data = big_mult;
			out->write(data7);

			share_secret_loop:
			for(ap_uint<128> i = 1; i < secret; i++){
				//#pragma HLS loop_tripcount min=1 max=256 avg=128
    			//#pragma HLS pipeline
    			//#pragma HLS unroll factor = 1
				big_mult *= other_public_key;
			}
			//big_mult = big_mult % modulus;

			/*while (big_mult >= modulus) {
				#pragma HLS pipeline
				big_mult = big_mult - modulus;
			}*/

			galapagos_packet data3;
			                data3.dest = 0;
			                data3.id = 2;
			                data3.keep = KEEP_ALL;
			                data3.last = 1;
			                data3.data = other_public_key;
			out->write(data3);
			galapagos_packet data8;
			                data8.dest = 0;
			                data8.id = 2;
			                data8.keep = KEEP_ALL;
			                data8.last = 1;
			                data8.data = modulus;
			out->write(data8);
			galapagos_packet data9;
			                data9.dest = 0;
			                data9.id = 2;
			                data9.keep = KEEP_ALL;
			                data9.last = 1;
			                data9.data = big_mult;
			out->write(data9);

			big_mult_t f1 = big_mult / modulus;
			big_mult_t f2 = f1 * modulus;
			big_mult = big_mult - f2;

			shared_secret = big_mult;

			galapagos_packet data1;
			                data1.dest = 0;
			                data1.id = 2;
			                data1.keep = KEEP_ALL;
			                data1.last = 1;
			                data1.data = f1;
			out->write(data1);
			galapagos_packet data2;
			                data2.dest = 0;
			                data2.id = 2;
			                data2.keep = KEEP_ALL;
			                data2.last = 1;
			                data2.data = f2;
			out->write(data2);
			galapagos_packet data4;
			                data4.dest = 0;
			                data4.id = 2;
			                data4.keep = KEEP_ALL;
			                data4.last = 1;
			                data4.data = big_mult;
			out->write(data4);

			#ifdef ENABLE_DEBUG_PRINT
			std::cout << "CORE generated shared secret: " << shared_secret << std::endl;
			#endif

			time_step++;
			trans_done = true;
			// final_key = shared_secret;

            // it should send to the corresponding AES kernel
            if (id == 1) {
                // out->packet_write((char *)&shared_secret, 1, 3 /*rec ENC id*/, 1 /*send RSA id*/);
                galapagos_packet data;
                data.dest = 3;
                data.id = 1;
                data.keep = KEEP_ALL;
                data.last = 1;
                data.data = shared_secret;
                out->write(data);
				//printf("\n shared secret key from rsa 1: %x \n", (unsigned int)shared_secret);
            }

            else if (id == 2) {
                // out->packet_write((char *)&shared_secret, 1, 4 /*rec DEC id*/, 2 /*send RSA id*/);
                galapagos_packet data;
                data.dest = 4;
                data.id = 2;
                data.keep = KEEP_ALL;
                data.last = 1;
                data.data = shared_secret;
                out->write(data);
				//printf("\n shared secret key from rsa 2: %x \n", (unsigned int)shared_secret);
            }
            }

		}
	}
}
