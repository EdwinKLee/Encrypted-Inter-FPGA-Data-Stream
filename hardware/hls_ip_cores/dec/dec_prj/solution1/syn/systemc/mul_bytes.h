// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2019.1
// Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

#ifndef _mul_bytes_HH_
#define _mul_bytes_HH_

#include "systemc.h"
#include "AESL_pkg.h"


namespace ap_rtl {

struct mul_bytes : public sc_module {
    // Port declarations 4
    sc_out< sc_logic > ap_ready;
    sc_in< sc_lv<5> > a_V;
    sc_in< sc_lv<8> > b_V;
    sc_out< sc_lv<8> > ap_return;


    // Module declarations
    mul_bytes(sc_module_name name);
    SC_HAS_PROCESS(mul_bytes);

    ~mul_bytes();

    sc_trace_file* mVcdFile;

    sc_signal< sc_lv<1> > trunc_ln19_fu_66_p1;
    sc_signal< sc_lv<4> > trunc_ln19_1_fu_70_p1;
    sc_signal< sc_lv<4> > select_ln19_fu_74_p3;
    sc_signal< sc_lv<1> > tmp_fu_92_p3;
    sc_signal< sc_lv<5> > shl_ln841_fu_86_p2;
    sc_signal< sc_lv<1> > tmp_1_fu_116_p3;
    sc_signal< sc_lv<6> > shl_ln841_1_fu_108_p3;
    sc_signal< sc_lv<6> > select_ln19_2_fu_124_p3;
    sc_signal< sc_lv<1> > tmp_2_fu_144_p3;
    sc_signal< sc_lv<7> > shl_ln841_2_fu_136_p3;
    sc_signal< sc_lv<1> > tmp_3_fu_168_p3;
    sc_signal< sc_lv<8> > shl_ln841_3_fu_160_p3;
    sc_signal< sc_lv<3> > trunc_ln841_fu_184_p1;
    sc_signal< sc_lv<8> > shl_ln841_4_fu_188_p3;
    sc_signal< sc_lv<1> > tmp_4_fu_196_p3;
    sc_signal< sc_lv<8> > or_ln719_fu_204_p2;
    sc_signal< sc_lv<1> > tmp_5_fu_218_p3;
    sc_signal< sc_lv<8> > select_ln25_fu_210_p3;
    sc_signal< sc_lv<8> > shl_ln841_5_fu_234_p2;
    sc_signal< sc_lv<1> > tmp_6_fu_240_p3;
    sc_signal< sc_lv<8> > xor_ln719_fu_248_p2;
    sc_signal< sc_lv<1> > tmp_7_fu_262_p3;
    sc_signal< sc_lv<8> > select_ln25_1_fu_254_p3;
    sc_signal< sc_lv<8> > shl_ln841_6_fu_278_p2;
    sc_signal< sc_lv<1> > tmp_8_fu_284_p3;
    sc_signal< sc_lv<8> > xor_ln719_1_fu_292_p2;
    sc_signal< sc_lv<1> > tmp_9_fu_306_p3;
    sc_signal< sc_lv<8> > select_ln25_2_fu_298_p3;
    sc_signal< sc_lv<5> > zext_ln19_fu_82_p1;
    sc_signal< sc_lv<5> > select_ln19_1_fu_100_p3;
    sc_signal< sc_lv<5> > xor_ln19_fu_322_p2;
    sc_signal< sc_lv<7> > zext_ln19_1_fu_132_p1;
    sc_signal< sc_lv<7> > select_ln19_3_fu_152_p3;
    sc_signal< sc_lv<7> > xor_ln19_1_fu_332_p2;
    sc_signal< sc_lv<7> > zext_ln19_2_fu_328_p1;
    sc_signal< sc_lv<7> > xor_ln19_2_fu_338_p2;
    sc_signal< sc_lv<8> > select_ln19_4_fu_176_p3;
    sc_signal< sc_lv<8> > select_ln19_5_fu_226_p3;
    sc_signal< sc_lv<8> > select_ln19_6_fu_270_p3;
    sc_signal< sc_lv<8> > select_ln19_7_fu_314_p3;
    sc_signal< sc_lv<8> > xor_ln19_4_fu_354_p2;
    sc_signal< sc_lv<8> > xor_ln19_3_fu_348_p2;
    sc_signal< sc_lv<8> > xor_ln19_5_fu_360_p2;
    sc_signal< sc_lv<8> > zext_ln19_3_fu_344_p1;
    static const sc_logic ap_const_logic_1;
    static const bool ap_const_boolean_1;
    static const sc_lv<4> ap_const_lv4_0;
    static const sc_lv<5> ap_const_lv5_1;
    static const sc_lv<32> ap_const_lv32_1;
    static const sc_lv<5> ap_const_lv5_0;
    static const sc_lv<2> ap_const_lv2_0;
    static const sc_lv<32> ap_const_lv32_2;
    static const sc_lv<6> ap_const_lv6_0;
    static const sc_lv<3> ap_const_lv3_0;
    static const sc_lv<32> ap_const_lv32_3;
    static const sc_lv<7> ap_const_lv7_0;
    static const sc_lv<32> ap_const_lv32_4;
    static const sc_lv<8> ap_const_lv8_0;
    static const sc_lv<8> ap_const_lv8_1B;
    static const sc_lv<32> ap_const_lv32_5;
    static const sc_lv<8> ap_const_lv8_1;
    static const sc_lv<32> ap_const_lv32_7;
    static const sc_lv<32> ap_const_lv32_6;
    static const sc_logic ap_const_logic_0;
    // Thread declarations
    void thread_ap_ready();
    void thread_ap_return();
    void thread_or_ln719_fu_204_p2();
    void thread_select_ln19_1_fu_100_p3();
    void thread_select_ln19_2_fu_124_p3();
    void thread_select_ln19_3_fu_152_p3();
    void thread_select_ln19_4_fu_176_p3();
    void thread_select_ln19_5_fu_226_p3();
    void thread_select_ln19_6_fu_270_p3();
    void thread_select_ln19_7_fu_314_p3();
    void thread_select_ln19_fu_74_p3();
    void thread_select_ln25_1_fu_254_p3();
    void thread_select_ln25_2_fu_298_p3();
    void thread_select_ln25_fu_210_p3();
    void thread_shl_ln841_1_fu_108_p3();
    void thread_shl_ln841_2_fu_136_p3();
    void thread_shl_ln841_3_fu_160_p3();
    void thread_shl_ln841_4_fu_188_p3();
    void thread_shl_ln841_5_fu_234_p2();
    void thread_shl_ln841_6_fu_278_p2();
    void thread_shl_ln841_fu_86_p2();
    void thread_tmp_1_fu_116_p3();
    void thread_tmp_2_fu_144_p3();
    void thread_tmp_3_fu_168_p3();
    void thread_tmp_4_fu_196_p3();
    void thread_tmp_5_fu_218_p3();
    void thread_tmp_6_fu_240_p3();
    void thread_tmp_7_fu_262_p3();
    void thread_tmp_8_fu_284_p3();
    void thread_tmp_9_fu_306_p3();
    void thread_tmp_fu_92_p3();
    void thread_trunc_ln19_1_fu_70_p1();
    void thread_trunc_ln19_fu_66_p1();
    void thread_trunc_ln841_fu_184_p1();
    void thread_xor_ln19_1_fu_332_p2();
    void thread_xor_ln19_2_fu_338_p2();
    void thread_xor_ln19_3_fu_348_p2();
    void thread_xor_ln19_4_fu_354_p2();
    void thread_xor_ln19_5_fu_360_p2();
    void thread_xor_ln19_fu_322_p2();
    void thread_xor_ln719_1_fu_292_p2();
    void thread_xor_ln719_fu_248_p2();
    void thread_zext_ln19_1_fu_132_p1();
    void thread_zext_ln19_2_fu_328_p1();
    void thread_zext_ln19_3_fu_344_p1();
    void thread_zext_ln19_fu_82_p1();
};

}

using namespace ap_rtl;

#endif