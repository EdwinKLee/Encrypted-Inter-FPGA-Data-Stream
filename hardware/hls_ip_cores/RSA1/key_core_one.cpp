#define INTERFACE_100G

#include "key_core_one.hpp"
#include "cstddef"


void key_core_one
 (
  short id,
  galapagos_interface * in,
  galapagos_interface  * out,
  ap_uint<16> &timeStep
  ){
#pragma HLS_DATAFLOW
#pragma HLS INTERFACE ap_ctrl_none port=return
     
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
     
     size_t size;
     short dest, src;
     
     timeStep.write(time_step);
     
     if(trans_done)
         return;
     
     //Load in the destination (i.e. who we are generating a key with)
     if( (time_step == 0) && !in->empty() ){
         //Retrieve data and add to the sum
         
         galapagos_packet data;
         data = in->read();
         src = data.id;
         
         if (src == 0) {
             key_exchange_partner_dest = (ap_uint<32>)data.data;
             
             secret = 1 + id;
             time_step++;
         }
     }
     
     //Sender/receiver logic needs to be split for the first few steps
     else if(time_step <= 2){
         if(is_sender){
             //Modulus  generation
             if(time_step == 1){
                 
                 //Generate modulus and send it
                 modulus = 7;
                 const data out_data = {key_exchange_partner_dest, modulus, 1, 0};
                 
                 // it should send to the other RSA kernel
                 
                 galapagos_packet data;
                 data.dest = 2;
                 data.id = 1;
                 data.keep = KEEP_ALL;
                 data.last = 1;
                 data.data = out_data.value;
                 out->write(data);
                 
                 time_step++;
             }
             
             else if(time_step == 2){
                 
                 //Generate base and send it
                 base = 23;
                 const data out_data = {key_exchange_partner_dest, base, 1, 0};
                 
                 // it should send to the other RSA kernel
                 
                 galapagos_packet data;
                 data.dest = 2;
                 data.id = 1;
                 data.keep = KEEP_ALL;
                 data.last = 1;
                 data.data = out_data.value;
                 out->write(data);
                 
                 time_step++;
             }
         }
         
         else{
             // nothing here, because it is the sender
         }
     }
     
     //Latter time steps - send/receive phase is done
     else{
         //Public key generation
         if(time_step == 3){
             
             //public_key = pow(base, secret) % self.modulus
#ifdef ENABLE_DEBUG_PRINT
             std::cout << "CORE generating pub key with:\n\tbase: " << base << "\n\tsecret: " << secret << "\n\tmod: " << modulus << std::endl;
#endif
             
             big_mult_t big_mult = base;
             
         pub_key_loop:
             for(ap_uint<128> i = 1; i < secret; i++){
#pragma HLS loop_tripcount min=1 max=256 avg=128
#pragma HLS pipeline
#pragma HLS unroll factor = 1
                 big_mult *= base;
             }
             big_mult = (big_mult % modulus);
             
#ifdef ENABLE_DEBUG_PRINT
             std::cout << "CORE generated pub key: " << public_key << std::endl;
#endif
             const data out_data = {key_exchange_partner_dest, public_key, 1, 0};
             
             if (id == 1) {
                 galapagos_packet data;
                 data.dest = 2;
                 data.id = 1;
                 data.keep = KEEP_ALL;
                 data.last = 1;
                 data.data = out_data.value;
                 out->write(data);
             }
             else if (id == 2) {
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
             
             ap_uint<128> other_public_key;
             size_t size;
             short dest, src;
             
             galapagos_packet data;
             data = in->read();
             src = data.id;
             if (src == 2) {
                 other_public_key = (ap_uint<128>)(data.data);
                 
                 //shared_secret = pow(other_public_key, secret) % modulus
#ifdef ENABLE_DEBUG_PRINT
                 std::cout << "CORE generating shared secret with:\n\tother pub: " << other_public_key << "\n\tsecret: " << secret << "\n\tmod: " << modulus << std::endl;
#endif
                 
                 big_mult_t big_mult = other_public_key;
                 
             share_secret_loop:
                 for(ap_uint<128> i = 1; i < secret; i++){
#pragma HLS loop_tripcount min=1 max=256 avg=128
#pragma HLS pipeline
#pragma HLS unroll factor = 1
                     big_mult *= other_public_key;
                 }
                 big_mult = big_mult % modulus;
                 
                 shared_secret = big_mult;
                 
#ifdef ENABLE_DEBUG_PRINT
                 std::cout << "CORE generated shared secret: " << shared_secret << std::endl;
#endif
                 
                 time_step++;
                 trans_done = true;
                 
                 // it should send to the corresponding AES kernel
                 if (id == 1) {
                     galapagos_packet data;
                     data.dest = 3;
                     data.id = 1;
                     data.keep = KEEP_ALL;
                     data.last = 1;
                     data.data = shared_secret;
                     out->write(data);
                 }
                 
                 else if (id == 2) {
                     galapagos_packet data;
                     data.dest = 4;
                     data.id = 2;
                     data.keep = KEEP_ALL;
                     data.last = 1;
                     data.data = shared_secret;
                     out->write(data);
                 }
             }
         }
     }
 }

