# 4.2寸三色墨水屏（400×300） + 驱动转接板

## 硬件说明

- 屏幕：三色墨水屏，发货前经过测试；**转接板排针默认没焊接**，需要焊排针要提前跟卖家说。
- 使用方式：需要搭配你自己的ESP系列开发板（ESP32 / ESP8266 / ESP32‑C3 / ESP32‑S3）来驱动。
  - 不包含：主控、电池。 

### Arduino驱动配置（用GxEPD2库）

- 屏幕对应驱动类：`GxEPD2_420c_GDEY042Z98`

  > ⚠️不要选微雪4.2 V2的驱动（那是黑白版本）

- 步骤： ①安装Arduino IDE ②库管理器安装 **GxEPD2（作者ZinggJM）** ③安装ESP开发板SDK（网上教程很多） ④示例代码里修改驱动型号，对照商品页接线图接好线即可。

### 资料链接

- [微雪参考手册](https://www.waveshare.net/wiki/4.2inch_e-Paper_Module)

- [演示固件烧录](https://www.bilibili.com/video/BV1eiZJYAEDV )
- [ESP驱动墨水屏](https://www.bilibili.com/video/BV1xx4y1s7y9 )
- [墨水屏图片显示](https://www.bilibili.com/video/BV1exUdYVEQm)
- [墨水屏合集](https://e.v9.cx/)

### 注意点

- ESP内存不能太小，墨水屏刷新需要较大内存；小内存主控实现起来难度高。
- 套餐二是成品固件方案（时钟、日历、图片轮播、小程序传图），**套餐一没有预烧录固件，全部需要自己写代码**。
- 百度网盘说明文档链接：https://pan.baidu.com/s/1CQH84yVHHmiz5NqmOnbN4w?pwd=lulu 提取码 lulu

## 第三方资料

[大连佳显](https://www.good-display.cn/product/378.html)
