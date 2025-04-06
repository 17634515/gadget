var fields = clazz.getClass().getDeclaredFields();
for(var i=0;i<fields.length;i++){
    var field = fields[i];
    field.setAccessible(true);
    console.log(field.getName(),field.get(clazz))
}

// package com.eastmoney.android.activity

// class StockActivity {

//         /* static fields */
//         static boolean D; => false
//         static java.math.BigDecimal U; => [0x84e6]: 100
//         static com.eastmoney.android.data.c g; => [0x8202]: K.TRADE_ACCOUNT_CHANGED
//         static com.eastmoney.android.data.c h; => [0x81fa]: K.STOCK_CONFIG_CHANGED

//         /* instance fields */
//         java.util.ArrayList A; => [0x81e2]: [com.eastmoney.android.activity.StockActivity$3{2a32ed4 VFED..... ........ 0,0-1080,1984}]
//         com.eastmoney.android.ui.StockItem$a B; => [0x81da]: com.eastmoney.android.activity.StockActivity$1@fffe427
//         android.view.View$OnClickListener C; => [0x81ca]: com.eastmoney.android.activity.StockActivity$13@4a236d4
//         java.util.concurrent.atomic.AtomicBoolean E; => [0x81ba]: true
//         boolean F; => false
//         int G; => 0
//         boolean H; => false
//         androidx.lifecycle.MutableLiveData I; => [0x81aa]: androidx.lifecycle.MutableLiveData@1be207d
//         java.lang.String J; => 66.30
//         java.lang.String K; => 66.30
//         java.lang.String L; => 66.30
//         java.lang.String M; => -5.88%
//         java.lang.String N; => -4.14
//         java.lang.String O; => 77.48
//         java.lang.String P; => 63.40
//         java.lang.String Q; => 70.44
//         int R; => -16736256
//         java.lang.String[] S; => 66.30,66.29,66.28,66.27,66.26
//         java.lang.String[] T; => 66.31,66.32,66.33,66.34,66.35
//         java.lang.String V; => null
//         java.lang.String W; => null
//         java.lang.String X; => --年份收益
//         java.lang.String Y; => 7日年化
//         int Z; => 6
//         java.lang.String a; => 0.00
//         android.view.View aA; => [0x819a]: android.widget.LinearLayout{f55ec40 V.E...... ........ 0,0-809,135 #7f090584 app:id/dock_bar_normal}
//         android.view.View aB; => [0x818a]: android.widget.LinearLayout{f114779 G.E...... ......I. 0,0-0,0 #7f090582 app:id/dock_bar_deal}
//         android.view.View aC; => [0x817a]: android.widget.LinearLayout{eb87cbe G.E...... ......I. 0,0-0,0 #7f090583 app:id/dock_bar_deal_liang_rong}
//         com.eastmoney.android.stockdetail.http.bean.e$a aD; => null
//         com.eastmoney.android.stockdetail.http.bean.c$a aE; => null
//         com.eastmoney.android.stockdetail.http.bean.d$a aF; => null
//         android.text.SpannableStringBuilder aG; => [0x816a]:
//         boolean aH; => false
//         int aI; => 0
//         com.eastmoney.android.dialog.DockMoreDialog aJ; => null
//         com.eastmoney.android.dialog.DockDealDialog aK; => null
//         com.eastmoney.android.stockdetail.fragment.IndexPopupDialogFragment aL; => null
//         com.eastmoney.android.ui.DimPopupWindow aM; => null
//         com.eastmoney.stock.bean.Stock aN; => [0x815a]: STOCK[StockName: 上证指数, codeWithMarket: SH000001, stockType: 1, useFrame: ]
//         com.eastmoney.android.stockdetail.fragment.chart.StockTitleChartFragment aO; => [0x814a]: StockTitleChartFragment{fe29de4 (5f1e9066-ae98-4a10-a5f0-cdb8b954a0b1) id=0x7f09058e StockTitleChartFragment}
//         com.eastmoney.stock.bean.Stock aP; => [0x813a]: STOCK[StockName: 通信设备, codeWithMarket: BI0448, stockType: -1, useFrame: ]
//         com.eastmoney.android.ui.MarqueeLayout aQ; => [0x812a]: com.eastmoney.android.ui.MarqueeLayout{383311f GFED..C.. ......I. 0,135-1080,135 #7f090f69 app:id/marquee_layout}
//         long aR; => 4
//         java.util.List aS; => [0x811a]: []
//         com.eastmoney.android.ui.StockConfigToolView aT; => null
//         com.eastmoney.android.util.Mask aU; => null
//         boolean aV; => false
//         boolean aW; => false
//         androidx.fragment.app.Fragment aX; => null
//         androidx.fragment.app.Fragment aY; => null
//         androidx.fragment.app.Fragment aZ; => null
//         java.lang.String aa; => 72.93
//         java.lang.String ab; => 59.67
//         int ac; => 0
//         int ad; => -1
//         java.lang.String ae; => Asia/Shanghai
//         boolean af; => false
//         int ag; => 3
//         int ah; => 0
//         com.eastmoney.stock.bean.BKDetailIntro ai; => null
//         int aj; => -1
//         com.eastmoney.android.util.cb ak; => [0x810a]: com.eastmoney.android.util.cb@4cd8c6c
//         com.eastmoney.android.dialog.b al; => [0x80fa]: com.eastmoney.android.dialog.b@3d5e235
//         android.os.Handler am; => [0x80ea]: Handler (android.os.Handler) {6fba6ca}
//         java.lang.Object an; => null
//         boolean ao; => false
//         int ap; => 0
//         com.eastmoney.android.chart.a$a aq; => [0x80da]: com.eastmoney.android.activity.StockActivity$10@33c03b
//         android.widget.TextView ar; => [0x80ca]: android.widget.TextView{33d0358 VFED..C.. ........ 606,0-809,135 #7f09057f app:id/dock_add}
//         android.widget.TextView as; => [0x80ba]: com.eastmoney.android.ui.DotTextView{60f6cb1 VFED..C.. ........ 0,0-202,135 #7f090585 app:id/dock_deal}
//         android.widget.ImageView at; => [0x80aa]: android.widget.ImageView{9956596 GFED..C.. ......ID 0,0-0,0 #7f090588 app:id/dock_deal_guide}
//         android.widget.TextView au; => [0x809a]: android.widget.TextView{1942517 GFED..C.. ......ID 0,0-0,0 #7f09058d app:id/dock_fund_deal}
//         android.widget.TextView av; => [0x808a]: android.widget.TextView{62afd04 GFED..C.. ......I. 0,0-0,0 #7f090591 app:id/dock_otc_deal_aip}
//         android.widget.TextView aw; => [0x807a]: android.widget.TextView{e7d22ed G.ED..... ......ID 0,0-0,0 #7f090590 app:id/dock_otc_aip}
//         android.widget.TextView ax; => [0x806a]: android.widget.TextView{c3a4522 G.ED..... ......ID 0,0-0,0 #7f090594 app:id/dock_tax_rate}
//         android.widget.TextView ay; => [0x805a]: android.widget.TextView{2bfbb3 GFED..C.. ......ID 0,0-0,0 #7f090592 app:id/dock_pk}
//         android.widget.TextView az; => [0x804a]: android.widget.TextView{8dce570 GFED..C.. ......ID 0,0-0,0 #7f090580 app:id/dock_aip}
//         java.lang.String b; => 0.00
//         com.eastmoney.android.base.stock.d$a ba; => [0x803a]: com.eastmoney.android.activity.StockActivity$56@89600e9
//         com.eastmoney.android.base.stock.d$a bb; => [0x802a]: com.eastmoney.android.activity.StockActivity$57@95b916e
//         com.eastmoney.android.base.stock.d$a bc; => [0x801a]: com.eastmoney.android.activity.StockActivity$59@b60a00f
//         android.content.BroadcastReceiver c; => [0x800a]: com.eastmoney.android.activity.StockActivity$73@f3b46aa
//         android.content.DialogInterface$OnClickListener d; => [0x7ffa]: com.eastmoney.android.activity.StockActivity$40@382e89c
//         android.content.DialogInterface$OnClickListener e; => [0x7fea]: com.eastmoney.android.activity.StockActivity$41@644c2a5
//         android.content.DialogInterface$OnClickListener f; => [0x7fda]: com.eastmoney.android.activity.StockActivity$42@f33567a
//         int i; => 1080
//         android.widget.RelativeLayout j; => [0x7fca]: android.widget.RelativeLayout{9912e2b V.E...... ........ 0,0-1080,2255 #7f0915f0 app:id/stock_rl}
//         com.eastmoney.android.ui.ptrlayout.EMPtrLayout k; => [0x7fba]: com.eastmoney.android.ui.ptrlayout.EMPtrLayout{f23f288 V.E...... ........ 0,135-1080,2119 #7f0911cf app:id/ptr_frame_layout}
//         androidx.viewpager.widget.ViewPager l; => [0x7faa]: com.eastmoney.android.ui.FixedViewPager{363e421 VFED..... ........ 0,0-1080,1984 #7f0920b1 app:id/viewPager}
//         com.eastmoney.android.activity.StockActivity$c m; => [0x7f9a]: com.eastmoney.android.activity.StockActivity$c@3906046
//         android.view.ViewGroup n; => [0x7f8a]: android.widget.RelativeLayout{df28207 V.E...... ........ 0,0-1080,135 #7f09186a app:id/titlebar_root}
//         com.eastmoney.android.ui.ActTipView o; => null
//         com.eastmoney.android.ui.ActTipView p; => null
//         com.eastmoney.android.config.BottomTabButton q; => null
//         boolean r; => false
//         com.eastmoney.stock.bean.Stock s; => [0x7f7a]: STOCK[StockName: 剑桥科技, codeWithMarket: SH603083, stockType: 2, useFrame: ]
//         com.eastmoney.android.ui.StockItem t; => [0x7f6a]: com.eastmoney.android.activity.StockActivity$3{2a32ed4 VFED..... ........ 0,0-1080,1984}
//         boolean u; => false
//         com.eastmoney.stock.bean.NearStockManager v; => [0x7f5a]: com.eastmoney.stock.bean.NearStockManager@c49af34
//         com.eastmoney.android.config.BottomTabButton w; => null
//         int x; => 1
//         boolean y; => false
//         boolean z; => false

//         /* constructor methods */
//         com.eastmoney.android.activity.StockActivity();

//         /* static methods */
//         static android.widget.TextView A(com.eastmoney.android.activity.StockActivity);
//         static void B(com.eastmoney.android.activity.StockActivity);
//         static android.widget.TextView C(com.eastmoney.android.activity.StockActivity);
//         static android.os.Handler D(com.eastmoney.android.activity.StockActivity);
//         static void E(com.eastmoney.android.activity.StockActivity);
//         static void F(com.eastmoney.android.activity.StockActivity);
//         static void G(com.eastmoney.android.activity.StockActivity);
//         static void H(com.eastmoney.android.activity.StockActivity);
//         static void I(com.eastmoney.android.activity.StockActivity);
//         static void J(com.eastmoney.android.activity.StockActivity);
//         static androidx.fragment.app.Fragment K(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.base.stock.d$a L(com.eastmoney.android.activity.StockActivity);
//         static void M(com.eastmoney.android.activity.StockActivity);
//         static void N(com.eastmoney.android.activity.StockActivity);
//         static int O(com.eastmoney.android.activity.StockActivity);
//         static boolean P(com.eastmoney.android.activity.StockActivity);
//         static boolean Q(com.eastmoney.android.activity.StockActivity);
//         static void R(com.eastmoney.android.activity.StockActivity);
//         static void S(com.eastmoney.android.activity.StockActivity);
//         static void T(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.ui.DimPopupWindow U(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.dialog.DockMoreDialog V(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String W(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String X(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String Y(com.eastmoney.android.activity.StockActivity);
//         static void Z(com.eastmoney.android.activity.StockActivity);
//         static int a(com.eastmoney.android.activity.StockActivity, int);
//         static long a(com.eastmoney.android.activity.StockActivity, long);
//         static androidx.fragment.app.Fragment a(com.eastmoney.android.activity.StockActivity, androidx.fragment.app.Fragment);
//         static com.eastmoney.android.ui.ActTipView a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.ui.ActTipView);
//         static com.eastmoney.stock.bean.BKDetailIntro a(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.BKDetailIntro);
//         static com.eastmoney.stock.bean.Stock a(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String a(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void a(com.eastmoney.android.activity.StockActivity, int, int);
//         static void a(com.eastmoney.android.activity.StockActivity, int, com.eastmoney.stock.bean.Stock);
//         static void a(com.eastmoney.android.activity.StockActivity, android.view.View);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.adv2.bean.AdItem);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.data.d);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.data.d, int);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.data.d, int, int);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.stockdetail.http.bean.c, java.lang.String);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.stockdetail.http.bean.d, java.lang.String);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.stockdetail.http.bean.e, java.lang.String);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.stockdetail.http.bean.f, java.lang.String);
//         static void a(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static void a(com.eastmoney.android.activity.StockActivity, java.lang.String, java.lang.String, int, boolean);
//         static void a(com.eastmoney.android.activity.StockActivity, java.lang.String, boolean);
//         static void a(com.eastmoney.android.activity.StockActivity, boolean, boolean);
//         static void a(java.lang.String);
//         static void a(java.lang.String, java.lang.String);
//         static boolean a(com.eastmoney.android.activity.StockActivity, boolean);
//         static boolean aa(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String ab(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String ac(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String ad(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String[] ae(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String[] af(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String ag(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String ah(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.ui.ActTipView ai(com.eastmoney.android.activity.StockActivity);
//         static android.widget.RelativeLayout aj(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.stockdetail.fragment.chart.StockTitleChartFragment ak(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.stock.bean.Stock al(com.eastmoney.android.activity.StockActivity);
//         static void am(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.util.Mask an(com.eastmoney.android.activity.StockActivity);
//         static boolean ao(com.eastmoney.android.activity.StockActivity);
//         static boolean ap(com.eastmoney.android.activity.StockActivity);
//         static android.os.Handler aq(com.eastmoney.android.activity.StockActivity);
//         static void ar(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String as(com.eastmoney.android.activity.StockActivity);
//         static int at(com.eastmoney.android.activity.StockActivity);
//         static int au(com.eastmoney.android.activity.StockActivity);
//         static int av(com.eastmoney.android.activity.StockActivity);
//         static int b(com.eastmoney.android.activity.StockActivity, int);
//         static androidx.fragment.app.Fragment b(com.eastmoney.android.activity.StockActivity, androidx.fragment.app.Fragment);
//         static com.eastmoney.android.ui.StockItem b(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String b(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void b(com.eastmoney.android.activity.StockActivity, com.eastmoney.android.data.d);
//         static void b(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static void b(com.eastmoney.android.activity.StockActivity, boolean, boolean);
//         static boolean b(com.eastmoney.android.activity.StockActivity, boolean);
//         static int c(com.eastmoney.android.activity.StockActivity, int);
//         static androidx.fragment.app.Fragment c(com.eastmoney.android.activity.StockActivity, androidx.fragment.app.Fragment);
//         static java.lang.String c(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void c(com.eastmoney.android.activity.StockActivity);
//         static void c(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static void c(com.eastmoney.android.activity.StockActivity, boolean);
//         static int d(com.eastmoney.android.activity.StockActivity, int);
//         static java.lang.String d(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static java.util.ArrayList d(com.eastmoney.android.activity.StockActivity);
//         static void d(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static void d(com.eastmoney.android.activity.StockActivity, boolean);
//         static int e(com.eastmoney.android.activity.StockActivity, int);
//         static com.eastmoney.android.config.BottomTabButton e(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String e(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void e(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static void e(com.eastmoney.android.activity.StockActivity, boolean);
//         static int f(com.eastmoney.android.activity.StockActivity, int);
//         static com.eastmoney.stock.bean.Stock f(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static java.lang.String f(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static boolean f(com.eastmoney.android.activity.StockActivity);
//         static boolean f(com.eastmoney.android.activity.StockActivity, boolean);
//         static com.eastmoney.android.ui.ptrlayout.EMPtrLayout g(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.stock.bean.Stock g(com.eastmoney.android.activity.StockActivity, com.eastmoney.stock.bean.Stock);
//         static java.lang.String g(com.eastmoney.android.activity.StockActivity, int);
//         static java.lang.String g(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static boolean g(com.eastmoney.android.activity.StockActivity, boolean);
//         static android.graphics.Bitmap h(com.eastmoney.android.activity.StockActivity, boolean);
//         static com.eastmoney.android.activity.StockActivity$c h(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String h(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void h(com.eastmoney.android.activity.StockActivity, int);
//         static android.widget.TextView i(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String i(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static void i(com.eastmoney.android.activity.StockActivity, int);
//         static boolean i(com.eastmoney.android.activity.StockActivity, boolean);
//         static int j(com.eastmoney.android.activity.StockActivity, int);
//         static android.view.View j(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String j(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static android.view.View k(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String k(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static java.math.BigDecimal k();
//         static void k(com.eastmoney.android.activity.StockActivity, int);
//         static int l(com.eastmoney.android.activity.StockActivity, int);
//         static android.view.View l(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String l(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static com.eastmoney.stock.bean.BKDetailIntro m(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String m(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static java.lang.String n(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String n(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static int o(com.eastmoney.android.activity.StockActivity);
//         static void o(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static int p(com.eastmoney.android.activity.StockActivity);
//         static void p(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static java.lang.String q(com.eastmoney.android.activity.StockActivity);
//         static void q(com.eastmoney.android.activity.StockActivity, java.lang.String);
//         static java.lang.String r(com.eastmoney.android.activity.StockActivity);
//         static int s(com.eastmoney.android.activity.StockActivity);
//         static java.lang.String t(com.eastmoney.android.activity.StockActivity);
//         static void u(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.stockdetail.http.bean.d$a v(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.stockdetail.http.bean.e$a w(com.eastmoney.android.activity.StockActivity);
//         static com.eastmoney.android.stockdetail.bean.PriceBoardData x(com.eastmoney.android.activity.StockActivity);
//         static android.widget.TextView y(com.eastmoney.android.activity.StockActivity);
//         static android.widget.TextView z(com.eastmoney.android.activity.StockActivity);

//         /* instance methods */
//         void A();
//         void B();
//         void C();
//         void D();
//         void E();
//         void F();
//         void G();
//         void H();
//         void I();
//         void J();
//         void K();
//         void L();
//         void M();
//         void N();
//         void O();
//         void P();
//         void Q();
//         void R();
//         void S();
//         void T();
//         void U();
//         void V();
//         void W();
//         void X();
//         void Y();
//         boolean Z();
//         java.lang.String a(com.eastmoney.android.config.BottomTabButton);
//         void a(int, int);
//         void a(int, com.eastmoney.stock.bean.Stock);
//         void a(int, java.util.List, android.view.View);
//         void a(int, boolean, java.lang.String);
//         void a(android.text.SpannableStringBuilder);
//         void a(android.view.View);
//         void a(com.eastmoney.android.adv2.bean.AdItem);
//         void a(com.eastmoney.android.data.d);
//         void a(com.eastmoney.android.data.d, int);
//         void a(com.eastmoney.android.data.d, int, int);
//         void a(com.eastmoney.android.stockdetail.http.bean.c$a);
//         void a(com.eastmoney.android.stockdetail.http.bean.c, java.lang.String);
//         void a(com.eastmoney.android.stockdetail.http.bean.d, java.lang.String);
//         void a(com.eastmoney.android.stockdetail.http.bean.e$a);
//         void a(com.eastmoney.android.stockdetail.http.bean.e, java.lang.String);
//         void a(com.eastmoney.android.stockdetail.http.bean.f, java.lang.String);
//         void a(com.eastmoney.stock.bean.Stock, java.lang.String);
//         void a(java.lang.String, java.lang.String, int, boolean);
//         void a(java.lang.String, java.lang.String, java.lang.String);
//         void a(java.lang.String, boolean);
//         void a(java.lang.VerifyError);
//         void a(java.util.List);
//         void a(boolean);
//         void a(boolean, boolean);
//         void a(java.lang.String[], android.content.DialogInterface$OnClickListener);
//         boolean a(android.content.Intent);
//         boolean a(android.os.Bundle);
//         boolean a(android.view.View, com.eastmoney.stock.bean.Stock);
//         androidx.fragment.app.Fragment a(int, java.lang.Class, java.lang.String);
//         com.eastmoney.stock.bean.Stock a();
//         void a(int);
//         void a(int, com.eastmoney.android.stockdetail.fragment.chart.StockChart);
//         void a(int, boolean, java.lang.String, com.eastmoney.stock.bean.Stock, java.lang.String, java.lang.String, java.lang.String, java.lang.String[], java.lang.String[], java.lang.String, int, java.lang.String, java.lang.String, java.lang.String, java.lang.String, int, int);
//         void a(int, boolean, java.lang.String, com.eastmoney.stock.bean.Stock, java.lang.String, java.lang.String, java.lang.String, java.lang.String[], java.lang.String[], java.lang.String, int, java.lang.String, java.lang.String, java.lang.String, java.lang.String, int, int, int);
//         void a(int, boolean, java.lang.String, com.eastmoney.stock.bean.Stock, java.lang.String, java.lang.String, java.lang.String, java.lang.String[], java.lang.String[], java.lang.String, java.lang.String);
//         void a(android.net.Uri);
//         void a(com.eastmoney.stock.bean.Stock);
//         boolean a(int, java.lang.String);
//         boolean aa();
//         void ab();
//         void ac();
//         void ad();
//         void ae();
//         void af();
//         void ag();
//         void b(int);
//         void b(int, com.eastmoney.stock.bean.Stock);
//         void b(int, boolean, java.lang.String);
//         void b(android.content.Intent);
//         void b(android.net.Uri);
//         void b(android.view.View);
//         void b(com.eastmoney.android.config.BottomTabButton);
//         void b(com.eastmoney.android.data.d);
//         void b(com.eastmoney.stock.bean.Stock);
//         void b(com.eastmoney.stock.bean.Stock, java.lang.String);
//         void b(boolean);
//         void b(boolean, boolean);
//         boolean b(android.view.View, com.eastmoney.stock.bean.Stock);
//         com.eastmoney.android.ui.StockItem b();
//         boolean b(java.lang.String);
//         java.lang.String c(int);
//         void c(com.eastmoney.stock.bean.Stock);
//         void c(boolean);
//         com.eastmoney.android.ui.ptrlayout.EMPtrLayout c();
//         void c(java.lang.String);
//         void d(com.eastmoney.stock.bean.Stock);
//         void d(boolean);
//         boolean d(int);
//         void d(java.lang.String);
//         boolean d();
//         android.graphics.Bitmap e(boolean);
//         void e(int);
//         void e(com.eastmoney.stock.bean.Stock);
//         void e(java.lang.String);
//         void e();
//         void f(int);
//         void f(com.eastmoney.stock.bean.Stock);
//         void f(java.lang.String);
//         void f();
//         void g(int);
//         void g(com.eastmoney.stock.bean.Stock);
//         void g(java.lang.String);
//         void g();
//         void h(int);
//         void h(com.eastmoney.stock.bean.Stock);
//         void h(java.lang.String);
//         void h();
//         void i(int);
//         void i(com.eastmoney.stock.bean.Stock);
//         void i(java.lang.String);
//         void i();
//         void j(com.eastmoney.stock.bean.Stock);
//         boolean j();
//         com.eastmoney.android.lib.job.jobs.Job k(com.eastmoney.stock.bean.Stock);
//         void l(com.eastmoney.stock.bean.Stock);
//         boolean l();
//         void m();
//         void n();
//         void o();
//         void p();
//         void q();
//         void r();
//         void s();
//         void t();
//         void u();
//         void v();
//         com.eastmoney.android.stockdetail.bean.PriceBoardData w();
//         void x();
//         void y();
//         void z();
//         boolean closeProgress();
//         void handleDealItemClick(android.view.View);
//         void handleDockClickEvent(android.view.View);
//         void handleHKDealItemClick(android.view.View);
//         void handleHistoryChartClickEvent(android.view.View);
//         void handleLiangRongDealItemClick(android.view.View);
//         void handleMoreItemClick(android.view.View);
//         void handleNormalItemClick(android.view.View);
//         void handleTitleChartClickEvent(android.view.View);
//         void onActivityResult(int, int, android.content.Intent);
//         void onClick(android.view.View);
//         void onConfigurationChanged(android.content.res.Configuration);
//         void onCreate(android.os.Bundle);
//         void onDestroy();
//         void onEvent(com.eastmoney.android.closedwarn.bean.a);
//         void onEvent(com.elbbbird.android.socialsdk.otto.ShareBusEvent);
//         void onEvent(com.sdk.aek.a);
//         void onIndexClicked(int);
//         void onIndexClickedAgain(int);
//         boolean onKeyDown(int, android.view.KeyEvent);
//         void onPageScrollStateChanged(int);
//         void onPageScrolled(int, float, int);
//         void onPageSelected(int);
//         void onPause();
//         void onRestoreInstanceState(android.os.Bundle);
//         void onResume();
//         void onSaveInstanceState(android.os.Bundle);
//         void onSetStatusBar(android.app.Activity);
//         void startProgress();

// }