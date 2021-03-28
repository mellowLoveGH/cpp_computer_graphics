#pragma once

#ifndef COMPONENT__H
#define COMPONENT__H

//各节点相对其父节点局部坐标系变换分量

//胳膊
//平移变换分量
#define ATX -150
#define ATY 20
#define ATZ -750
//绕指定轴变换分量
#define ATHETA (PI)
#define ARX 0
#define ARY 1
#define ARZ 0
//放缩分量
#define ASX 0.77
#define ASY 0.77
#define ASZ 0.77

//手
//平移变换分量
#define HTX 90
#define HTY 0
#define HTZ -800
//绕指定轴变换分量
#define HTHETA 0
#define HRX 0
#define HRY 1
#define HRZ 0
//放缩分量
#define HSX 1
#define HSY 1
#define HSZ 1

#endif
