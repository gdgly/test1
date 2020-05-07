
#ifndef APPS_SUB_PHY_H
#define APPS_SUB_PHY_H

/*
 * fun: [关闭充电盒]，耳机进入in_case状态
 */
void subPhyEnterCase(void);

/*
 * fun: [打开充电盒]，耳机进入out_case状态
 */
void subPhyExitCase(void);

/*
 * fun: [从充电盒中取出]，进入空中
 */
void subPhyEnterAir(void);

/*
 * fun: 耳机从空中[放入充电盒]
 */
void subPhyExitAir(void);

/*
 * fun: [从充电盒中取出]，进入空中
 */
void subPhyEnterEar(void);

/*
 * fun: 耳机从空中[放入充电盒]
 */
void subPhyExitEar(void);

#endif //APPS_SUB_PHY_H
