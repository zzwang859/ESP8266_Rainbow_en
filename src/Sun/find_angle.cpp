#include <Arduino.h>
#include "../Epaper/epaper.h"
#include "find_angle.h"

/*****************************************************************************************************************
  Global variable city list
 *****************************************************************************************************************/
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE) 
static const _CITY City_list[345] ICACHE_RODATA_ATTR  = {
{"WX4FBXXFKE4F", 116.4, 39.9,}, /* Beijing/Beijing */
{"WWGQDCW6TBW1", 117.2, 39.12,}, /* Tianjin/Tianjin */
{"WWC2MYYCM6J5", 114.52, 38.05,}, /* Hebei/Shijiazhuang/Shijiazhuang */
{"WXH13WQT7XKN", 118.2, 39.63,}, /* Hebei/Tangshan/Tangshan */
{"WXJ51T411GRX", 119.6, 39.93,}, /* Hebei/Qinhuangdao/Qinhuangdao */
{"WW92M43YCQG0", 114.48, 36.62,}, /* Hebei/Handan/Handan */
{"WW96VH3U3KG5", 114.48, 37.07,}, /* Hebei/Xingtai */
{"WWFJ5S4DNJ0T", 115.47, 38.87,}, /* Hebei/Baoding/Baoding */
{"WX38NPJ1DP88", 114.88, 40.82,}, /* Hebei/Zhangjiakou/Zhangjiakou */
{"WX7C5D6581T3", 117.93, 40.97,}, /* Hebei/Chengde/Chengde */
{"WWG1FS318M6P", 116.83, 38.3,}, /* Hebei/Cangzhou/Cangzhou */
{"WX4BZD6KEJFY", 116.7, 39.52,}, /* Hebei/Langfang/Langfang */
{"WWDQ8KE95MGB", 115.68, 37.73,}, /* Hebei/Hengshui/Hengshui */
{"WW8P3NH2TPDT", 112.55, 37.87,}, /* Shanxi/Taiyuan/Taiyuan */
{"WX0S40QDCME0", 113.3, 40.08,}, /* Shanxi/Datong/Datong */
{"WW8Z267HHZ3U", 113.57, 37.85,}, /* Shanxi/Yangquan */
{"WW2MYN52R11E", 113.12, 36.2,}, /* Shanxi/Changzhi/Changzhi */
{"WW23BS9T8PB1", 112.8755951, 35.4895724,}, /* Shanxi/Jincheng */
{"WQZZWRV1BGKH", 112.43, 39.33,}, /* Shanxi/Shuozhou/Shuozhou */
{"WW8NMEQJCMJE", 112.75, 37.68,}, /* Shanxi/Jinzhong/Jinzhong */
{"WQNZJRJNM3JV", 110.98, 35.02,}, /* Shanxi/Yuncheng/Yuncheng */
{"WWB4T4PKXRH7", 112.73, 38.42,}, /* Shanxi/Xinzhou/Xinzhou */
{"WQRM38NMHR7P", 111.52, 36.08,}, /* Shanxi/Linfen/Linfen */
{"WQXJ2UGJEGPU", 111.1310005, 37.512405,}, /* Shanxi/Lüliang/Lüliang */
{"WRR2Q2Z7CXWM", 111.73, 40.83,}, /* Inner Mongolia/Hohhot/Hohhot */
{"WRNP70PG2MPX", 109.83, 40.65,}, /* Inner Mongolia/Baotou/Baotou */
{"WR5CWTXYQRVP", 106.82, 39.67,}, /* Inner Mongolia/Wuhai/Wuhai */
{"WXS86P4BSG4G", 118.92, 42.27,}, /* Inner Mongolia/Chifeng/Chifeng */
{"WXYBNKF2CT25", 122.27, 43.62,}, /* Inner Mongolia/Tongliao/Tongliao */
{"WRN16S9GJRQ1", 109.8, 39.62,}, /* Inner Mongolia/Ordos/Ordos */
{"WXRVB9QYXKY8", 123.43, 41.8,}, /* Liaoning/Shenyang/Shenyang */
{"WWYMRT0VRMUG", 121.62, 38.92,}, /* Liaoning/Dalian/Dalian */
{"WXR3Y6P2CEQ3", 122.98, 41.1,}, /* Liaoning/Anshan/Anshan */
{"WZ2NM0P2XMU7", 123.98, 41.88,}, /* Liaoning/Fushun */
{"WZ24BQQTFN10", 123.77, 41.3,}, /* Liaoning/Benxi/Benxi */
{"WZ0KQ36Y0N4N", 124.38, 40.13,}, /* Liaoning/Dandong/Dandong */
{"WXQ1U6H0VE63", 121.13, 41.1,}, /* Liaoning/Jinzhou/Jinzhou */
{"WXNZM7TPVMMZ", 122.23, 40.67,}, /* Liaoning/Yingkou/Yingkou */
{"WXQX09T2HRXX", 121.67, 42.02,}, /* Liaoning/Fuxin */
{"WXRDFBCXJJ8E", 123.17, 41.27,}, /* Liaoning/Liaoyang/Liaoyang */
{"WXQCCV8TM5T4", 122.07, 41.12,}, /* Liaoning/Panjin/Panjin */
{"WZ809BF6B5HR", 123.83, 42.28,}, /* Liaoning/Tieling */
{"WXMSKZT4B3TT", 120.45, 41.57,}, /* Liaoning/Chaoyang */
{"WXJZTSCEPN4J", 120.83, 40.72,}, /* Liaoning/Huludao/Huludao */
{"WZC1EXZ0P9HU", 125.32, 43.9,}, /* Jilin/Changchun/Changchun */
{"WZCCRENBK8PQ", 126.55, 43.83,}, /* Jilin/Jilin/Jilin */
{"WZ8MTDUGMP22", 124.35, 43.17,}, /* Jilin/Siping/Siping */
{"WZ8GZQHFH76E", 125.13, 42.88,}, /* Jilin/Liaoyuan/Liaoyuan */
{"WZ3T3SEQVEVF", 125.93, 41.73,}, /* Jilin/Tonghua/Tonghua */
{"WZ3YSC05ZW82", 126.42, 41.93,}, /* Jilin/Baishan/Baishan */
{"YB0B8REJKCCB", 124.82, 45.13,}, /* Jilin/Songyuan/Songyuan */
{"Y8P7E0F5F2QK", 122.83, 45.62,}, /* Jilin/Baicheng/Baicheng */
{"YB1UX38K6DY1", 126.53, 45.8,}, /* Heilongjiang/Harbin/Harbin */
{"YB2JK81THR6Z", 123.95, 47.33,}, /* Heilongjiang/Qiqihar/Qiqihar */
{"YBJ1SQ97WHN1", 130.97, 45.3,}, /* Heilongjiang/Jixi/Jixi */
{"YBKTK25TPP2R", 130.27, 47.33,}, /* Heilongjiang/Hegang/Hegang */
{"YBM322SX2QPB", 131.15, 46.63,}, /* Heilongjiang/Shuangyashan/Shuangyashan */
{"YB2BVPSH4JM5", 125.03, 46.58,}, /* Heilongjiang/Daqing/Daqing */
{"YB7XT0YP7SYK", 128.9, 47.73,}, /* Heilongjiang/Yichun */
{"YBKDQEKD30TF", 130.37, 46.82,}, /* Heilongjiang/Jiamusi/Jiamusi */
{"YBJH7VFZU4T1", 130.95, 45.78,}, /* Heilongjiang/Qitaihe/Qitaihe */
{"WZUJT5EC6FK4", 129.6, 44.58,}, /* Heilongjiang/Mudanjiang/Mudanjiang */
{"YBFTUGSP7QCT", 127.48, 50.25,}, /* Heilongjiang/Heihe/Heihe */
{"YB63388P2QN8", 126.98, 46.63,}, /* Heilongjiang/Suihua/Suihua */
{"YBBQB6TMV95Z", 124.12, 50.42,}, /* Heilongjiang/Daxinganling/Daxinganling */
{"WTW3SJ5ZBJUY", 121.47, 31.23,}, /* Shanghai/Shanghai */
{"WTSQQYHVQ973", 118.78, 32.07,}, /* Jiangsu/Nanjing/Nanjing */
{"WTTE97PU94T9", 120.3, 31.57,}, /* Jiangsu/Wuxi/Wuxi */
{"WW56FQXV5ZHB", 117.18, 34.27,}, /* Jiangsu/Xuzhou/Xuzhou */
{"WTTKC92GVT6S", 119.95, 31.78,}, /* Jiangsu/Changzhou/Changzhou */
{"WTTDPCGXTWUS", 120.58, 31.3,}, /* Jiangsu/Suzhou/Suzhou */
{"WTTVYTY1H0HF", 120.88, 31.98,}, /* Jiangsu/Nantong/Nantong */
{"WWHUBFTRE77P", 119.22, 34.6,}, /* Jiangsu/Lianyungang/Lianyungang */
{"WTUXK27DXN0R", 119.02, 33.62,}, /* Jiangsu/Huai'an/Huai'an */
{"WTVMWP4TCFT7", 120.15, 33.35,}, /* Jiangsu/Yancheng/Yancheng */
{"WTUBM40RTTUB", 119.4, 32.4,}, /* Jiangsu/Yangzhou/Yangzhou */
{"WTSZNJUFDEP9", 119.45, 32.2,}, /* Jiangsu/Zhenjiang/Zhenjiang */
{"WTV28G74FW3J", 119.92, 32.45,}, /* Jiangsu/Taizhou/Taizhou */
{"WWH1780CEFBR", 118.28, 33.97,}, /* Jiangsu/Suqian/Suqian */
{"WTMKQ069CCJ7", 120.15, 30.28,}, /* Zhejiang/Hangzhou/Hangzhou */
{"WTQ3VZQP879N", 121.55, 29.88,}, /* Zhejiang/Ningbo/Ningbo */
{"WSVZ695PCF71", 120.7, 28,}, /* Zhejiang/Wenzhou/Wenzhou */
{"WTMYGTZ7WZMY", 120.75, 30.75,}, /* Zhejiang/Jiaxing/Jiaxing */
{"WTMRU3PTDC3X", 120.08, 30.9,}, /* Zhejiang/Huzhou/Huzhou */
{"WTMDXTK6STK4", 120.57, 30,}, /* Zhejiang/Shaoxing/Shaoxing */
{"WTJJ6TY7R0FK", 119.65, 29.08,}, /* Zhejiang/Jinhua/Jinhua */
{"WTHS8FW3RYBK", 118.87, 28.93,}, /* Zhejiang/Quzhou/Quzhou */
{"WTQFSTR6DMK4", 122.2, 30,}, /* Zhejiang/Zhoushan/Zhoushan */
{"WTN74CS48NB0", 121.4154053, 28.661049,}, /* Zhejiang/Taizhou/Taizhou */
{"WTJ3BG5J683H", 119.92, 28.45,}, /* Zhejiang/Lishui/Lishui */
{"WTEMH46Z5N09", 117.25, 31.83,}, /* Anhui/Hefei/Hefei */
{"WTS4JZ3WMZMC", 118.38, 31.33,}, /* Anhui/Wuhu/Wuhu */
{"WTG7R0CSBHZ9", 117.38, 32.92,}, /* Anhui/Bengbu/Bengbu */
{"WTG1WKHG98G5", 117, 32.63,}, /* Anhui/Huainan/Huainan */
{"WTSK2DC1FHK3", 118.5, 31.7,}, /* Anhui/Ma'anshan/Ma'anshan */
{"WW511U7JJ68Q", 116.8, 33.95,}, /* Anhui/Huaibei/Huaibei */
{"WT7ZCN9UKCZS", 117.82, 30.93,}, /* Anhui/Tongling/Tongling */
{"WT7JXTC4NSMZ", 117.05, 30.53,}, /* Anhui/Anqing/Anqing */
{"WTK1H2DTJ1U4", 118.3154208, 29.710646,}, /* Anhui/Huangshan/Huangshan */
{"WTSPU2JCPU6T", 118.32, 32.3,}, /* Anhui/Chuzhou/Chuzhou */
{"WTF75T383SE2", 115.82, 32.9,}, /* Anhui/Fuyang/Fuyang */
{"WTGPMFNHZW6H", 116.98, 33.63,}, /* Anhui/Suzhou/Suzhou */
{"WTDUEP89WV0P", 116.5, 31.77,}, /* Anhui/Liu'an/Liu'an */
{"WW42DDHQT937", 115.78, 33.85,}, /* Anhui/Bozhou/Bozhou */
{"WT7W3R63DQMH", 117.48, 30.67,}, /* Anhui/Chizhou/Chizhou */
{"WTS2N4R0GD1V", 118.75, 30.95,}, /* Anhui/Xuancheng/Xuancheng */
{"WSSU6EXX52RE", 119.3, 26.08,}, /* Fujian/Fuzhou/Fuzhou */
{"WS7GQBRNR6V8", 118.08, 24.48,}, /* Fujian/Xiamen/Xiamen */
{"WSS8S6CQZBPG", 119.0169525, 25.4166562,}, /* Fujian/Putian/Putian */
{"WSETKW23BDUT", 117.62, 26.27,}, /* Fujian/Sanming/Sanming */
{"WSKMS3KBE2JM", 118.67, 24.88,}, /* Fujian/Quanzhou/Quanzhou */
{"WS7EMPWP7EQ3", 117.65, 24.52,}, /* Fujian/Zhangzhou/Zhangzhou */
{"WSSP952Z8846", 118.17, 26.65,}, /* Fujian/Nanping/Nanping */
{"WS7NZ16DHEHU", 117.03, 25.1,}, /* Fujian/Longyan/Longyan */
{"WSSZXXPU1B76", 119.52, 26.67,}, /* Fujian/Ningde/Ningde */
{"WT47HJP3HEMP", 115.85, 28.68,}, /* Jiangxi/Nanchang/Nanchang */
{"WT5QD22W4GKR", 117.17, 29.27,}, /* Jiangxi/Jingdezhen/Jingdezhen */
{"WSBVNTZH3M2C", 113.85, 27.63,}, /* Jiangxi/Pingxiang/Pingxiang */
{"WT62ZWSX5CM0", 116, 29.7,}, /* Jiangxi/Jiujiang/Jiujiang */
{"WSCWR06PNPFU", 114.92, 27.82,}, /* Jiangxi/Xinyu/Xinyu */
{"WT50ZFRU8TYY", 117.07, 28.27,}, /* Jiangxi/Yingtan/Yingtan */
{"WS9DZQ1QXHGR", 114.93, 25.83,}, /* Jiangxi/Ganzhou/Ganzhou */
{"WSCF23JCKNQJ", 114.98, 27.12,}, /* Jiangxi/Ji'an/Ji'an */
{"WSCQ4UBFXPTE", 114.38, 27.8,}, /* Jiangxi/Yichun/Yichun */
{"WSFXR95RZD21", 116.35, 28,}, /* Jiangxi/Fuzhou/Fuzhou */
{"WT5CU7PJK0RS", 117.97, 28.45,}, /* Jiangxi/Shangrao/Shangrao */
{"WWE0TGW4PX6N", 116.98, 36.67,}, /* Shandong/Jinan/Jinan */
{"WWMT5Q64CR3G", 120.38, 36.07,}, /* Shandong/Qingdao/Qingdao */
{"WWECQQCKH37Z", 118.05, 36.82,}, /* Shandong/Zibo/Zibo */
{"WW5QJDVC4NKQ", 117.32, 34.82,}, /* Shandong/Zaozhuang/Zaozhuang */
{"WWSKUMUVE3TR", 118.67, 37.43,}, /* Shandong/Dongying */
{"WWWM51WDH1FX", 121.43, 37.45,}, /* Shandong/Yantai/Yantai */
{"WWS8Z342UPZ7", 119.15, 36.7,}, /* Shandong/Weifang/Weifang */
{"WW6CSB4BQYSD", 116.58, 35.42,}, /* Shandong/Jining/Jining */
{"WW7MBNP039PE", 117.08, 36.2,}, /* Shandong/Taian/Taian */
{"WWWV6YM1F4ZV", 122.12, 37.52,}, /* Shandong/Weihai/Weihai */
{"WWKCX8PB3YE6", 119.52, 35.42,}, /* Shandong/Rizhao/Rizhao */
{"WWHPMHEV4CS4", 118.35, 35.05,}, /* Shandong/Linyi/Linyi */
{"WWDTN3X443VR", 116.3, 37.45,}, /* Shandong/Dezhou/Dezhou */
{"WW6RR5W09UKP", 115.98, 36.45,}, /* Shandong/Liaocheng/Liaocheng */
{"WWEUSKZ4K1P9", 117.97, 37.38,}, /* Shandong/Binzhou/Binzhou */
{"WW607X60CDS4", 115.469125, 35.2401634,}, /* Shandong/Heze/Heze */
{"WW0V9QP93VS8", 113.62, 34.75,}, /* Henan/Zhengzhou/Zhengzhou */
{"WW1MBZNSXN1M", 114.3, 34.8,}, /* Henan/Kaifeng/Kaifeng */
{"WQPUYY7S4GV2", 112.45, 34.62,}, /* Henan/Luoyang/Luoyang */
{"WW02P7XHT0KB", 113.18, 33.77,}, /* Henan/Pingdingshan/Pingdingshan */
{"WW3M6GBCXPV8", 114.38, 36.1,}, /* Henan/Anyang */
{"WW372S04V500", 114.28, 35.75,}, /* Henan/Hebi/Hebi */
{"WW2BZF5MQKGW", 113.9, 35.3,}, /* Henan/Xinxiang */
{"WW2835D5K7BE", 113.25, 35.22,}, /* Henan/Jiaozuo/Jiaozuo */
{"WW3G3XDR3729", 115.03, 35.77,}, /* Henan/Puyang */
{"WW0CWDZP17BC", 113.85, 34.03,}, /* Henan/Xuchang/Xuchang */
{"WTCP494CDTDN", 114.02, 33.58,}, /* Henan/Luohe/Luohe */
{"WQPJF7T25VCT", 111.2, 34.78,}, /* Henan/Sanmenxia/Sanmenxia */
{"WTB58RQWFJC5", 112.52, 33,}, /* Henan/Nanyang/Nanyang */
{"WW45ZXMVB36N", 115.65, 34.45,}, /* Henan/Shangqiu/Shangqiu */
{"WT9NG9P4ZDHG", 114.07, 32.13,}, /* Henan/Xinyang/Xinyang */
{"WTCX2BMDDWHP", 114.65, 33.62,}, /* Henan/Zhoukou/Zhoukou */
{"WTC5DEFF6TD5", 114.02, 32.98,}, /* Henan/Zhumadian/Zhumadian */
{"WW0P9B6381U6", 112.58, 35.07,}, /* Henan/Jiyuan/Jiyuan */
{"WT3Q0FW9ZJ3Q", 114.3, 30.6,}, /* Hubei/Wuhan/Wuhan */
{"WT3GC9DRC209", 115.03, 30.2,}, /* Hubei/Huangshi/Huangshi */
{"WMYC8VK3H18J", 110.7811547, 32.6364417,}, /* Hubei/Shiyan/Shiyan */
{"WMRNSHZ87YY1", 111.28, 30.7,}, /* Hubei/Yichang/Yichang */
{"WMXY3PVX93RV", 112.2, 32.08,}, /* Hubei/Xiangyang/Xiangyang */
{"WT3SYNJHF48X", 114.88, 30.4,}, /* Hubei/Ezhou/Ezhou */
{"WMXB90VDC7PZ", 112.2, 31.03,}, /* Hubei/Jingmen/Jingmen */
{"WT3PBQDH23US", 113.92, 30.93,}, /* Hubei/Xiaogan/Xiaogan */
{"WMRU9C7MH9C3", 112.23, 30.33,}, /* Hubei/Jingzhou */
{"WT3TJZMB152J", 114.87, 30.45,}, /* Hubei/Huanggang/Huanggang */
{"WT33C6J2C563", 114.32, 29.85,}, /* Hubei/Xianning/Xianning */
{"WT8S7Y3W2KX4", 113.37, 31.72,}, /* Hubei/Suizhou/Suizhou */
{"WMMU75CW7CKH", 109.47, 30.3,}, /* Hubei/Enshi */
{"WT2SV8EY0D76", 113.45, 30.37,}, /* Hubei/Xiantao/Xiantao */
{"WT2M09U9QEH8", 112.88, 30.42,}, /* Hubei/Qianjiang/Qianjiang */
{"WT2QRPRCSNKH", 113.17, 30.67,}, /* Hubei/Tianmen/Tianmen */
{"WMW7Q9T3W529", 110.332175, 31.517304,}, /* Hubei/Shennongjia/Shennongjia */
{"WT029G15ETRJ", 112.93, 28.23,}, /* Hunan/Changsha/Changsha */
{"WSBQQ66C8040", 113.13, 27.83,}, /* Hunan/Zhuzhou/Zhuzhou */
{"WSBQ3F31E8P0", 112.93, 27.83,}, /* Hunan/Xiangtan */
{"WSB118FZZ57Y", 112.57, 26.9,}, /* Hunan/Hengyang/Hengyang */
{"WKZ7089YZ20M", 111.47, 27.25,}, /* Hunan/Shaoyang/Shaoyang */
{"WT0RN4EKXHCD", 113.12, 29.37,}, /* Hunan/Yueyang */
{"WMPMM26US87X", 111.68, 29.05,}, /* Hunan/Changde/Changde */
{"WMNT9YCY99D6", 110.47, 29.13,}, /* Hunan/Zhangjiajie/Zhangjiajie */
{"WMPFEYKZJDQM", 112.32, 28.6,}, /* Hunan/Yiyang/Yiyang */
{"WS86EV4EKUK4", 113.02, 25.78,}, /* Hunan/Chenzhou/Chenzhou */
{"WKXQ7GRJYT07", 111.62, 26.43,}, /* Hunan/Yongzhou/Yongzhou */
{"WKYHZ4GWCU8B", 110, 27.57,}, /* Hunan/Huaihua/Huaihua */
{"WKZTU85FVNSV", 112, 27.73,}, /* Hunan/Loudi/Loudi */
{"WS0E9D8WN298", 113.27, 23.13,}, /* Guangdong/Guangzhou/Guangzhou */
{"WS2V1N2FZZ77", 113.6, 24.82,}, /* Guangdong/Shaoguan/Shaoguan */
{"WS10730EM8EV", 114.05, 22.55,}, /* Guangdong/Shenzhen/Shenzhen */
{"WEBY8Q5HHUCU", 113.57, 22.27,}, /* Guangdong/Zhuhai/Zhuhai */
{"WS4UZ4EZ953Z", 116.68, 23.35,}, /* Guangdong/Shantou/Shantou */
{"WS06YNEMPP18", 113.12, 23.02,}, /* Guangdong/Foshan/Foshan */
{"WS02MNW64CP6", 113.08, 22.58,}, /* Guangdong/Jiangmen/Jiangmen */
{"W7Y3P046TR7U", 110.35, 21.27,}, /* Guangdong/Zhanjiang/Zhanjiang */
{"W7YGK0CKZKF9", 110.92, 21.67,}, /* Guangdong/Maoming/Maoming */
{"WKPGPK45UZQM", 112.47, 23.05,}, /* Guangdong/Zhaoqing/Zhaoqing */
{"WS17E8Y5G0RU", 114.42, 23.12,}, /* Guangdong/Huizhou/Huizhou */
{"WS6D4KH12059", 116.12, 24.28,}, /* Guangdong/Meizhou/Meizhou */
{"WS4196CZ5GG1", 115.37, 22.78,}, /* Guangdong/Shantou */
{"WS1WFPCVR8WG", 114.7, 23.73,}, /* Guangdong/Heyuan/Heyuan */
{"W7ZSKJM3GP7Z", 111.98, 21.87,}, /* Guangdong/Yangjiang/Yangjiang */
{"WS0QU43YTBHW", 113.03, 23.7,}, /* Guangdong/Qingyuan/Qingyuan */
{"WS0GHKN5ZP7T", 113.75, 23.05,}, /* Guangdong/Dongguan/Dongguan */
{"WS08H58U92R8", 113.38, 22.52,}, /* Guangdong/Zhongshan/Zhongshan */
{"WEEJ79ZFGR3H", 116.8833425, 20.62047636,}, /* Guangdong/Dongsha/Dongsha */
{"WS4YTUBPDZY9", 116.62, 23.67,}, /* Guangdong/Chaozhou/Chaozhou */
{"WS4VBP4H9W1J", 116.37, 23.55,}, /* Guangdong/Jieyang/Jieyang */
{"WKPDMK3Y8JWT", 112.03, 22.92,}, /* Guangdong/Yunfu/Yunfu */
{"WKJ1F428HH2F", 108.37, 22.82,}, /* Guangxi/Nanning/Nanning */
{"WKMF3VJHWB9X", 109.42, 24.33,}, /* Guangxi/Liuzhou/Liuzhou */
{"WKQRV6P6WM81", 110.28, 25.28,}, /* Guangxi/Guilin/Guilin */
{"WKPJS42KFVJX", 111.27, 23.48,}, /* Guangxi/Wuzhou/Wuzhou */
{"W7VD5N6F061Q", 109.12, 21.48,}, /* Guangxi/Beihai/Beihai */
{"W7V53W659VPW", 108.35, 21.7,}, /* Guangxi/Fangchenggang/Fangchenggang */
{"W7VHZEYSJ2W6", 108.62, 21.95,}, /* Guangxi/Qinzhou/Qinzhou */
{"WKJGQJ0R3057", 109.6, 23.1,}, /* Guangxi/Guigang/Guigang */
{"WKN2DZXM71F0", 110.17, 22.63,}, /* Guangxi/Yulin/Yulin */
{"WK5ZFNY7D4FJ", 106.62, 23.9,}, /* Guangxi/Baise/Baise */
{"WKR6FMS0Z3UW", 111.55, 24.42,}, /* Guangxi/Hezhou/Hezhou */
{"WKKUE0QPVT8X", 108.07, 24.7,}, /* Guangxi/Hechi/Hechi */
{"WKJWVXFV72X7", 109.23, 23.73,}, /* Guangxi/guest/guest */
{"W7UR7MB3QQDU", 107.37, 22.4,}, /* Guangxi/Chongzuo/Chongzuo */
{"W7W3YQKE4QDH", 110.32, 20.03,}, /* Hainan/Haikou/Haikou */
{"W7JZGDR3YYTF", 109.5, 18.25,}, /* Hainan/Sanya/Sanya */
{"W6ZZU0E5REV5", 112.3286673, 16.8343799,}, /* Hainan/Sansha/Sansha */
{"W7MZJ3D2NP9X", 109.57, 19.52,}, /* Hainan/Danzhou/Danzhou */
{"W7MFU4Y12VWZ", 109.52, 18.78,}, /* Hainan/Wuzhishan/Wuzhishan */
{"W7QT9B3G3S46", 110.47, 19.25,}, /* Hainan/Qionghai/Qionghai */
{"W7QZ1QFP50HH", 110.8, 19.55,}, /* Hainan/Wenchang/Wenchang */
{"W7QDBNQVFQN8", 110.4, 18.8,}, /* Hainan/Wanning/Wanning */
{"W7MHXVJCW6NC", 108.63, 19.1,}, /* Hainan/Eastern/Eastern */
{"W7W2N6K8F64J", 110.32, 19.7,}, /* Hainan/Ding'an/Ding'an */
{"W7QQ1QHQ7VWQ", 110.1, 19.37,}, /* Hainan/Tunchang/Tunchang */
{"W7W0PPEX9F8F", 110, 19.73,}, /* Hainan/Chengmai/Chengmai */
{"W7TCRF6D3JPS", 109.68, 19.92,}, /* Hainan/Lingao/Lingao */
{"W7MV6SENCGG4", 109.45, 19.23,}, /* Hainan/Baisha/Baisha */
{"W7MT92RG3HJD", 109.05, 19.25,}, /* Hainan/Changjiang/Changjiang */
{"W7MDSJR4TMK4", 109.17, 18.75,}, /* Hainan/Ledong/Ledong */
{"W7Q1PZC6KCFW", 110.03, 18.5,}, /* Hainan/Lingshui/Lingshui */
{"W7Q1BR3PE78W", 109.7, 18.63,}, /* Hainan/Baoting/Baoting */
{"W7QH70RCB3PE", 109.83, 19.03,}, /* Hainan/Qiongzhong/Qiongzhong */
{"WM7B0X53DZW2", 106.55, 29.57,}, /* Chongqing/Chongqing */
{"WM6N2PM3WY2K", 104.07, 30.67,}, /* Sichuan/Chengdu/Chengdu */
{"WM4WBR42NUWC", 104.78, 29.35,}, /* Sichuan/Zigong/Zigong */
{"WK8R4WWPXZ1K", 101.72, 26.58,}, /* Sichuan/Panzhihua/Panzhihua */
{"WM4UPPEGCN1F", 105.43, 28.87,}, /* Sichuan/Luzhou/Luzhou */
{"WMD1P5P1QK95", 104.38, 31.13,}, /* Sichuan/Deyang/Deyang */
{"WMD7P0YN1CHN", 104.73, 31.47,}, /* Sichuan/Mianyang/Mianyang */
{"WMG22PXN3WPT", 105.83, 32.43,}, /* Sichuan/Guangyuan/Guangyuan */
{"WM7JD7CXZK37", 105.57, 30.52,}, /* Sichuan/Suining/Suining */
{"WM68Q2Z5FRTM", 105.05, 29.58,}, /* Sichuan/Neijiang/Neijiang */
{"WM3B1R518R80", 103.77, 29.57,}, /* Sichuan/Leshan/Leshan */
{"WM7RJGM47PV5", 106.08, 30.78,}, /* Sichuan/Nanchong/Nanchong */
{"WM3FFWQTYQ52", 103.83, 30.05,}, /* Sichuan/Meishan/Meishan */
{"WM47STMVQSWN", 104.62, 28.77,}, /* Sichuan/Yibin/Yibin */
{"WM7V66VSE5DV", 106.63, 30.47,}, /* Sichuan/Guang'an */
{"WMS3W5RJETCF", 107.5, 31.22,}, /* Sichuan/Dazhou/Dazhou */
{"WM34XCDQ1VND", 103, 29.98,}, /* Sichuan/Ya'an/Ya'an */
{"WMEVJW5E9Q66", 106.77, 31.85,}, /* Sichuan/Bazhong/Bazhong */
{"WM67M75V9WZY", 104.65, 30.12,}, /* Sichuan/Ziyang/Ziyang */
{"WMB74JR8MU82", 101.7, 32.9,}, /* Sichuan/Aba */
{"WJX5GH5PYG5R", 99.98, 31.62,}, /* Sichuan/Ganzi */
{"WKBWQZEYXGPF", 102.255249, 27.8597138,}, /* Sichuan/Liangshan/Liangshan */
{"WKEZD7MXE04F", 106.63, 26.65,}, /* Guizhou/Guiyang/Guiyang */
{"WKUJ8GKSN3ZC", 106.9144177, 27.7040959,}, /* Guizhou/Zunyi/Zunyi */
{"WKEM6FWMMFWJ", 105.95, 26.25,}, /* Guizhou/Anshun/Anshun */
{"WKFG79YD1BCG", 105.28, 27.3,}, /* Guizhou/Bijie/Bijie */
{"WKVTSQQ2WQS8", 109.18, 27.72,}, /* Guizhou/Tongren/Tongren */
{"WK3N92NQV6RQ", 102.72, 25.05,}, /* Yunnan/Kunming/Kunming */
{"WK9C440U729C", 103.8, 25.5,}, /* Yunnan/Qujing/Qujing */
{"WK2FT8F9H0YR", 102.55, 24.35,}, /* Yunnan/Yuxi/Yuxi */
{"WHQWBSVQS7RS", 99.17, 25.12,}, /* Yunnan/Baoshan/Baoshan */
{"WKCG2PQ8G1M2", 103.72, 27.33,}, /* Yunnan/Zhaotong/Zhaotong */
{"WHZ2BV37270V", 100.23, 26.88,}, /* Yunnan/Lijiang/Lijiang */
{"WHPG4ZZB3XSN", 101.03, 23.07,}, /* Yunnan/Pu'er/Pu'er */
{"WHPPV7HP148U", 100.08, 23.88,}, /* Yunnan/Lincang/Lincang */
{"WK2NQUD6QD0J", 101.55, 25.03,}, /* Yunnan/Chuxiong */
{"WK1KZS3MC5U1", 102.4206, 23.369161,}, /* Yunnan/Red River */
{"WK4HUQ2S15F0", 104.25, 23.37,}, /* Yunnan/Wenshan */
{"WHX38U36B3BU", 100.23, 25.6,}, /* Yunnan/Dali */
{"WHQ5514S00XF", 98.5727798, 24.4397767,}, /* Yunnan/Dehong/Dehong */
{"WHW713U75VQ2", 98.85, 25.85,}, /* Yunnan/Nujiang/Nujiang */
{"WJ2B9TWGFBT5", 91.13, 29.65,}, /* Tibet/Lhasa/Lhasa */
{"TVPNW82QN57R", 88.88, 29.27,}, /* Tibet/Shigatse/Shigatse */
{"WJT157H36HS5", 97.18, 31.13,}, /* Tibet/Chamdo/Chamdo */
{"WJ70G7N4S3H0", 94.37, 29.68,}, /* Tibet/Lingzhi/Lingzhi */
{"WJ1Q3KPFD1QS", 91.823482, 29.246042,}, /* Tibet/Shannan/Shannan */
{"WJ97P4G263RF", 92.07, 31.48,}, /* Tibet/Nagqu/Nagqu */
{"TV9JG0M1S9QU", 80.2951546, 31.9498805,}, /* Tibet/Ali/Ali */
{"WQJ6YY8MHZP0", 108.93, 34.27,}, /* Shaanxi/Xi'an/Xi'an */
{"WQJQWC27HZX1", 108.93, 34.9,}, /* Shaanxi/Tongchuan/Tongchuan */
{"WQH5TBCEVBMU", 107.13, 34.37,}, /* Shaanxi/Baoji/Baoji */
{"WQJ7398FTXFD", 108.7, 34.33,}, /* Shaanxi/Xianyang/Xianyang */
{"WQJU78X3NFJU", 109.5, 34.5,}, /* Shaanxi/Weinan/Weinan */
{"WQTB5QBFKYSB", 109.48, 36.6,}, /* Shaanxi/Yan'an/Yan'an */
{"WMUH5K1WDWZC", 107.02, 33.07,}, /* Shaanxi/Hanzhong/Hanzhong */
{"WQY1BBWVNM4Q", 109.73, 38.28,}, /* Shaanxi/Yulin/Yulin */
{"WMV9BV1YY1EC", 109.02, 32.68,}, /* Shaanxi/Ankang/Ankang */
{"WQN0TV21C6RY", 109.9402499, 33.8670433,}, /* Shaanxi/Shangluo/Shangluo */
{"WQ3V4QR6VR6G", 103.82, 36.07,}, /* Gansu/Lanzhou/Lanzhou */
{"WPJFKJQ5BCWZ", 98.27, 39.8,}, /* Gansu/Jiayuguan/Jiayuguan */
{"WQBEJ0TNY84M", 102.18, 38.5,}, /* Gansu/Jinchang/Jinchang */
{"WQ6PFTTX1VY6", 104.18, 36.55,}, /* Gansu/silver */
{"WQ5HTXNGNE1W", 105.72, 34.58,}, /* Gansu/Tianshui/Tianshui */
{"WQ8ZZ2UYK26X", 102.63, 37.93,}, /* Gansu/Wuwei/Wuwei */
{"WNZMMYCRWTUZ", 100.45, 38.93,}, /* Gansu/Zhangye/Zhangye */
{"WQ7F5V8HRBZU", 106.6885757, 35.5387592,}, /* Gansu/Pingliang/Pingliang */
{"WPN41UK0D3H6", 98.52, 39.75,}, /* Gansu/Jiuquan/Jiuquan */
{"WQKE38EK0NBP", 107.6485062, 35.7310098,}, /* Gansu/Qingyang/Qingyang */
{"WQ66KU06BHB1", 104.6231461, 35.5742283,}, /* Gansu/Dingxi/Dingxi */
{"WQ36SBF902FR", 103.22, 35.6,}, /* Gansu/Linxia */
{"WQ82K43YVYU8", 101.78, 36.62,}, /* Qinghai/Xining/Xining */
{"WQ2Y3RYEE7DY", 102.368689, 36.2980755,}, /* Qinghai/Haidong/Haidong */
{"WNXF241JU7T0", 100.9, 36.97,}, /* Qinghai/Haibei/Haibei */
{"WQ2D1TXVBJ01", 102.029686, 35.5390391,}, /* Qinghai/Huangnan/Huangnan */
{"WNRW6560ZURV", 100.6378555, 36.2728881,}, /* Qinghai/Hainan/Hainan */
{"WNPK15RJ0H84", 100.2488709, 34.4718606,}, /* Qinghai/Guoluo/Guoluo */
{"WJUGXXRYCVG7", 97.02, 33,}, /* Qinghai/Yushu */
{"WNTHRYRP0P8J", 97.3814393, 37.3451453,}, /* Qinghai/Haixi/Haixi */
{"WQGDF7NXZRZB", 106.28, 38.47,}, /* Ningxia/Yinchuan/Yinchuan */
{"WQGTUXRECXJN", 106.38, 39.02,}, /* Ningxia/Shizuishan/Shizuishan */
{"WQG80DH3X35S", 106.2, 37.98,}, /* Ningxia/Wuzhong/Wuzhong */
{"WQ7SF3WEPQRU", 106.28, 36,}, /* Ningxia/Guyuan/Guyuan */
{"WQDV3QM9ZDUM", 105.18, 37.52,}, /* Ningxia/Zhongwei/Zhongwei */
{"TZY33C4YJBP3", 87.62, 43.82,}, /* Xinjiang/Urumqi/Urumqi */
{"VBH77M0RQ24G", 84.87, 45.6,}, /* Xinjiang/Karamay */
{"TZXKM4G9GS71", 89.17, 42.95,}, /* Xinjiang/Turpan/Turpan */
{"WPDE8HGNVFWJ", 93.52, 42.83,}, /* Xinjiang/Hami/Hami */
{"TZY46T9GJ6Y0", 87.3, 44.02,}, /* Xinjiang/Changji */
{"TZ344WFBDS60", 80.27, 41.17,}, /* Xinjiang/Aksu */
{"TXN08CQCQM63", 75.98, 39.47,}, /* Xinjiang/Kashgar */
{"TY8G4T7ZWVX1", 79.92, 37.12,}, /* Xinjiang/Hotian */
{"VB71BQ84QPSS", 82.98, 46.75,}, /* Xinjiang/Tacheng */
{"VBW8JQY63YDB", 88.13, 47.85,}, /* Xinjiang/Altay */
{"TZVHJ8T4HQ93", 86.03, 44.3,}, /* Xinjiang/Shihezi/Shihezi */
{"TZ1Y9TFKC7BS", 81.28, 40.55,}, /* Xinjiang/Alar/Alar */
{"TZY7200FWT0M", 87.540372, 44.165496,}, /* Xinjiang/Wujiaqu/Wujiaqu */
{"WSJ89JC5P1S7", 120.28, 22.62,}, /* Taiwan/Kaohsiung */
{"WSQQMXBCC2VS", 121.531853, 25.04776,}, /* Taiwan/Taipei */
{"WECNV8ZNE40W", 114.157692, 22.285521,}, /* Hong Kong */
{"WEBTFUSKS7HB", 113.33, 22.13,}, /* Macau */
};
#endif
/*****************************************************************************************************************
  Global variables used to find the best attitude angle
 *****************************************************************************************************************/
FindAngleModule FindAngle;
// _BEST_RESULT &Best = FindAngle.Best;

/*****************************************************************************************************************
   Get the refractive index of light of a specified wavelength
 *****************************************************************************************************************/
double FindAngleModule::get_refract_n(double lamada)
{
    return (1.50805 + 2588.92498 / pow(lamada, 2) + 1.92102e8 / pow(lamada, 4)); // Calculate refractive index using Cauchy's formula
}

/*****************************************************************************************************************
  Find the angle between two unit vectors
 *****************************************************************************************************************/
double FindAngleModule::angle_of_vector(_VECTOR3* A, _VECTOR3* B)
{
    double angle = 0;

    angle += A->x * B->x;
    angle += A->y * B->y;
    angle += A->z * B->z;
    angle = acos(angle);

    return angle;
}

/*****************************************************************************************************************
  Generates orthogonal vectors of two non-parallel vectors
 *****************************************************************************************************************/
void FindAngleModule::vectgen_cross_vector(_VECTOR3* cross_vector, _VECTOR3* A, _VECTOR3* B)
{
    double abs_vector;

    cross_vector->x = A->y * B->z - A->z * B->y;
    cross_vector->y = A->z * B->x - A->x * B->z;
    cross_vector->z = A->x * B->y - A->y * B->x;

    //Converts a vector to a unit vector without changing its direction
    abs_vector = 0;
    abs_vector += cross_vector->x * cross_vector->x;
    abs_vector += cross_vector->y * cross_vector->y;
    abs_vector += cross_vector->z * cross_vector->z;
    abs_vector = sqrt(abs_vector);
    cross_vector->x /= abs_vector;
    cross_vector->y /= abs_vector;
    cross_vector->z /= abs_vector;
}

/*****************************************************************************************************************
   Generate rotation matrix
 *****************************************************************************************************************/
void FindAngleModule::gen_rotate_vector(_VECTOR3x3* rotate_vector, _VECTOR3* vector, double angle)
{
    double cosa, cos1_a, sina;
    double RxRx, RyRy, RzRz, RxRy, RxRz, RyRz;

    cosa = cos(angle);
    cos1_a = 1.0 - cosa;
    sina = sin(angle);

    RxRx = vector->x * vector->x;
    RyRy = vector->y * vector->y;
    RzRz = vector->z * vector->z;
    RxRy = vector->x * vector->y;
    RxRz = vector->x * vector->z;
    RyRz = vector->y * vector->z;

    rotate_vector->Vec[0].x = cosa + RxRx * cos1_a;
    rotate_vector->Vec[0].y = RxRy * cos1_a + vector->z * sina;
    rotate_vector->Vec[0].z = RxRz * cos1_a - vector->y * sina;
    // rotate_vector->Vec[0].a = 0;

    rotate_vector->Vec[1].x = RxRy * cos1_a - vector->z * sina;
    rotate_vector->Vec[1].y = cosa + RyRy * cos1_a;
    rotate_vector->Vec[1].z = RyRz * cos1_a + vector->x * sina;
    // rotate_vector->Vec[1].a = 0;

    rotate_vector->Vec[2].x = RxRz * cos1_a + vector->y * sina;
    rotate_vector->Vec[2].y = RyRz * cos1_a - vector->x * sina;
    rotate_vector->Vec[2].z = cosa + RzRz * cos1_a;
    // rotate_vector->Vec[2].a = 0;

    // rotate_vector->Vec[3].x = 0;
    // rotate_vector->Vec[3].y = 0;
    // rotate_vector->Vec[3].z = 0;
    // rotate_vector->Vec[3].a = 1;
}

/*****************************************************************************************************************
   Rotate a vector using a rotation matrix
****************************************************************************************************************/
void FindAngleModule::do_rotate_vector(_VECTOR3* out_vec, _VECTOR3x3* rotate_vector, _VECTOR3* in_vec)
{
    out_vec->x = rotate_vector->Vec[0].x * in_vec->x +
        rotate_vector->Vec[1].x * in_vec->y +
        rotate_vector->Vec[2].x * in_vec->z;

    out_vec->y = rotate_vector->Vec[0].y * in_vec->x +
        rotate_vector->Vec[1].y * in_vec->y +
        rotate_vector->Vec[2].y * in_vec->z;

    out_vec->z = rotate_vector->Vec[0].z * in_vec->x +
        rotate_vector->Vec[1].z * in_vec->y +
        rotate_vector->Vec[2].z * in_vec->z;
}

/******************************************************************************************************************
*  The sun's orientation is converted into a vector with modulus 1
* Consistent with the right-hand coordinate system of openGL
*              North y+ 0°
*               /|\
*                |
*                |
* 270°westx- --------------> East x+ 90°
*                |
*                |
*                |
*              South y- 180°
* Input parameters:
* azimuth solar altitude angle (-90~90 degrees)
* altitude solar azimuth angle (0~360 degrees) uses the coordinate system above
* windir window orientation (0~360 angle) uses the coordinate system above
* range traverse range and traversal step size

* Output parameters:
* max_refract2_rotate bubble sorting, the product of the maximum incident angle of the second refraction and the effective illumination area criterion
* max_refract2_inAngle The maximum incident angle of the second refraction and the corresponding rotation angle
* flag: 0=Normal 1: It is daytime, but the window has no light, 2: Night
*****************************************************************************************************************/
void FindAngleModule::scan_best_angle(_BEST_RESULT* best, _SCAN_PARAMETER* parm)
{
    _VECTOR3 Sun_vector, refract1_vector, prism_AB_dir, PrismLeft_vector, refract2A_vector;
    _VECTOR3 refract1_rotate1, prism_AB_rotate1, refract2A_rotate1, Sun_vector1, rotate_center;
    _VECTOR3x3 rotate_vector;

    double angle_sun_refract1, angle_AB_Dir, angle_sun_refract2, effect_light;
    double i, tmp, rotate_angle, delta_angle;

    // lamada = 350; // Visible light upper limit wavelength
    best->flag = 0;
    best->max_refract2_inAngle = 0;      // bubble sort
    best->max_refract2_rotate = -1;     // The maximum incident angle of the second refraction corresponds to the rotation angle

    if (parm->altitude > 0) {  // if it is daytime

        // azimuth = 2*PI-azimuth*PI/180.0;   // Azimuth angle converted to right-handed coordinate system radians
        // altitude = altitude*PI/180.0;      // Convert altitude angle to radians
        // windir = 2*PI-windir*PI/180.0;     // Window orientation converted to right-hand coordinate system radians

        tmp = parm->azimuth - parm->windir;
        if (tmp < 0) { tmp = tmp + 2 * PI; }

        if (tmp < PI / 2 || tmp>3 * PI / 2) {
            best->flag = 0; // Mark window with sunlight

            // n_zi  = get_refract_n('k9-glass', lamada); // Purple light 350 refractive index
            // linjie = asin(1/n_zi);                     // The critical angle when total reflection occurs

            /* Sunlight propagation direction space vector */
            Sun_vector.x = -cos(parm->altitude) * cos(parm->azimuth);
            Sun_vector.y = -cos(parm->altitude) * sin(parm->azimuth);
            Sun_vector.z = -sin(parm->altitude);

            /*********** Eigenvectors of a prism **************/
            /*First refractive surface Prism C surface External unit normal vector */
            refract1_vector.x = 0;
            refract1_vector.y = 0;
            refract1_vector.z = 1;

            /* Resolve the front and back of the prism. The south side is A and the north side is B, pointing to the front (south) direction of the prism. */
            prism_AB_dir.x = cos(parm->windir);
            prism_AB_dir.y = sin(parm->windir);
            prism_AB_dir.z = 0;

            /* Prism left direction space vector (rotation axis) */
            PrismLeft_vector.x = cos(parm->windir + PI / 2);
            PrismLeft_vector.y = sin(parm->windir + PI / 2);
            PrismLeft_vector.z = 0;

            /*  Second refractive surface Prism A surface Internal normal vector */
            refract2A_vector.x = -cos(PI / 6) * cos(parm->windir);
            refract2A_vector.y = -cos(PI / 6) * sin(parm->windir);
            refract2A_vector.z = 0.5;

            best->max_refract2_inAngle = 0;      // bubble sort
            best->max_refract2_rotate = -1;     // The maximum incident angle of the second refraction corresponds to the rotation angle

            /*****  Triangular prism rotation traversal **************************************/
            i = parm->range.start;
            do {
                //***** Generate prism rotation matrix ********
                rotate_angle = i / 180.0 * PI; // Angle of rotation, converted to radians
                gen_rotate_vector((_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&PrismLeft_vector, rotate_angle); // Generate prism rotation matrix

                // ****** CFace normal rotation**********
                do_rotate_vector((_VECTOR3*)&refract1_rotate1, (_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&refract1_vector);

                // ****** Prism front and rear logo rotation **********
                do_rotate_vector((_VECTOR3*)&prism_AB_rotate1, (_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&prism_AB_dir);

                // ****** Calculate the angle between the sunlight and the C-plane normal vector **************
                angle_sun_refract1 = angle_of_vector((_VECTOR3*)&Sun_vector, (_VECTOR3*)&refract1_rotate1);

                // ****** Calculate the angle between sunlight and the logo vectors before and after the prism **************
                //If the second refraction is less than 90 degrees and occurs on surface A, it will be deflected upward; if it is greater than 90 degrees, the second refraction occurs on surface B and it will be deflected downward.
                angle_AB_Dir = angle_of_vector((_VECTOR3*)&Sun_vector, (_VECTOR3*)&prism_AB_rotate1);

                /*****  If the back side is not illuminated and the second refraction occurs on side A  ************************/
                if ((angle_sun_refract1 > PI / 2) && (angle_AB_Dir < PI / 2)) {
                    // ****** Normal vector of the second reflecting surface A. Normal vector of surface A after rotating with the prism**
                    do_rotate_vector((_VECTOR3*)&refract2A_rotate1, (_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&refract2A_vector);

                    // ******* Calculate the refraction rotation axis and cross-multiply to obtain the orthogonal vector and normalize it*****
                    vectgen_cross_vector((_VECTOR3*)&rotate_center, (_VECTOR3*)&refract1_rotate1, (_VECTOR3*)&Sun_vector);

                    // ******* Calculate rotation angle based on refractive index **********************
                    rotate_angle = PI - angle_sun_refract1;                            // incident angle
                    delta_angle = rotate_angle - asin(sin(rotate_angle) / parm->n_zi);  // relative deflection angle

                    // ******* Rotate to get the first refracted ray ***********************
                    gen_rotate_vector((_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&rotate_center, delta_angle); // Generate refraction rotation matrix
                    do_rotate_vector((_VECTOR3*)&Sun_vector1, (_VECTOR3x3*)&rotate_vector, (_VECTOR3*)&Sun_vector);

                    // ****** Calculate the second refracted incident angle ***********************
                    angle_sun_refract2 = angle_of_vector((_VECTOR3*)&Sun_vector1, (_VECTOR3*)&refract2A_rotate1);
                    if (angle_sun_refract2 > PI / 2) {
                        angle_sun_refract2 = PI - angle_sun_refract2;
                    }
                    else {
                        angle_sun_refract2 = 0;
                        printf("error:second refract wrong\n");
                    }

                    // Determine whether total reflection occurs
                    if (angle_sun_refract2 > parm->linjie) {
                        angle_sun_refract2 = 0;
                    }

                    // Calculate the effective light input amount, that is, the orthographic projection of the C surface on the vertical surface in the direction of sunlight propagation
                    effect_light = -cos(angle_sun_refract1) * sin(angle_sun_refract2);

                    if (effect_light > best->max_refract2_inAngle) { // Bubble sorting finds the maximum value of the second incident angle and comprehensively evaluates the amount of incident light and the dispersion angle
                        best->max_refract2_inAngle = effect_light;
                        best->max_refract2_rotate = i;
                    }

                }
                else {
                    //         fprintf('Illumination to the back or secondary refraction on the B side\n');
                }
                i += parm->range.snap;
                yield(); /* Feed the dog to prevent the program from getting stuck */
            } while (i < parm->range.end);

        }
        else {  //if(tmp <PI/2 || tmp>3*PI/2 )  
            best->flag = 1; // Mark the window without sunlight, sleep for 30 minutes
        }

    }
    else { // if(parm->altitude > 0)  // if it is daytime
        best->flag = 2; // Marked in the dark period, sleep for 1 hour  
    }
}

/*****************************************************************************************************************
  Update the range and step size of the iteration traversal
 *****************************************************************************************************************/
void FindAngleModule::update_range(_BEST_RESULT* best, _SCAN_PARAMETER* parm, double new_snap)
{
    if (best->max_refract2_rotate - parm->range.snap < 0) {
        parm->range.start = 0;
    }
    else {
        parm->range.start = best->max_refract2_rotate - parm->range.snap;
    }

    if (best->max_refract2_rotate + parm->range.snap > 360) {
        parm->range.end = 360;
    }
    else {
        parm->range.end = best->max_refract2_rotate + parm->range.snap;
    }
    parm->range.snap = new_snap;
}

/*****************************************************************************************************************
  Find the best prism position at a certain moment
 *****************************************************************************************************************/
void FindAngleModule::find_best_angle(_BEST_RESULT* best, double zenith, double  azimuth, double windir)
{
    _SCAN_PARAMETER Parm;

    Parm.azimuth = 2 * PI - azimuth * PI / 180.0;   // Azimuth angle converted to right-handed coordinate system radians
    Parm.altitude = (90.0 - zenith) * PI / 180.0; // Convert altitude angle to radians
    Parm.windir = 2 * PI - windir * PI / 180.0;     // Window orientation converted to right-hand coordinate system radians
    Parm.n_zi = get_refract_n(LAMADA);     // Test light refractive index
    Parm.linjie = asin(1 / Parm.n_zi);        // The critical angle when total reflection occurs
    Parm.range.start = 0;
    Parm.range.end = 359;
    Parm.range.snap = 20;

    /* ** First rough traversal ******* 18times */
    scan_best_angle(best, (_SCAN_PARAMETER*)&Parm);

    /*** Second time traversal ******* 6 times   */
    if (best->flag == 0 && best->max_refract2_rotate != -1){
        update_range(best, (_SCAN_PARAMETER*)&Parm, 3.0);
        scan_best_angle(best, (_SCAN_PARAMETER*)&Parm);
    }

    /** The third time traversal *******  6 times   */
    if (best->flag == 0 && best->max_refract2_rotate != -1) {
        update_range(best, (_SCAN_PARAMETER*)&Parm, 1.0);
        scan_best_angle(best, (_SCAN_PARAMETER*)&Parm);
    }

    /** The fourth traversal *******  20 times  */
    if (best->flag == 0 && best->max_refract2_rotate != -1) {
        update_range(best, (_SCAN_PARAMETER*)&Parm, 0.1);
        scan_best_angle(best, (_SCAN_PARAMETER*)&Parm);
    }
}


/*****************************************************************************************************************************************
* Calculate the angle between the two places based on their longitude and latitude to evaluate the distance and traverse to find the nearest major city.
*****************************************************************************************************************************************/
#if(EINK_UI_LANGUAGE == EINK_LANGUAGE_CHINESE) 
unsigned  int  FindAngleModule::find_city(double ew, double sn)
{
    unsigned int  i, id;
    double angle, min_angle;
    _VECTOR3 A, B;

    A.z = cos(sn / 180.0 * PI);
    A.x = cos(ew / 180.0 * PI + PI) * A.z;
    A.y = sin(ew / 180.0 * PI + PI) * A.z;
    A.z = sin(sn / 180.0 * PI);

    /* Traverse to find the nearest node city */
    min_angle = PI;

    for (i = 0; i < sizeof(City_list) / sizeof(_CITY); i++) {
        yield(); /* Feed the dog to prevent the program from getting stuck */
 
        B.z = cos(City_list[i].sn / 180.0 * PI);
        B.x = cos(City_list[i].ew / 180.0 * PI + PI) * B.z;
        B.y = sin(City_list[i].ew / 180.0 * PI + PI) * B.z;
        B.z = sin(City_list[i].sn / 180.0 * PI);

        angle = angle_of_vector((_VECTOR3*)&A, (_VECTOR3*)&B);

        /* Bubble to find the nearest city */
        if (angle < min_angle) {
            min_angle = angle;
            id = i;
        }
    }

    return id;
}

const char* FindAngleModule::get_city_id(unsigned int city)
{
    return City_list[city].id;
}
#endif
