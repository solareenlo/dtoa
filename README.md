# dtoa [WIP]
- 自分なりの dtoa を作ってるよ．

## Reference
### 大元
- https://www.netlib.org/fp/dtoa.c
- https://www.netlib.org/fp/
- [jwiegley/gdtoa/dtoa.c](https://github.com/jwiegley/gdtoa/blob/master/dtoa.c)
	- 浮動小数点数とその10進表記の正確な相互変換に関するWilliam D Clingerの1990年の論文「[How to Read Floating Point Numbers Accurately](https://www.researchgate.net/profile/William_Clinger/publication/2295884_How_to_Read_Floating_Point_Numbers_Accurately/links/02e7e53624929eec6e000000/How-to-Read-Floating-Point-Numbers-Accurately.pdf)」と Guy L. Steele Jr. と Jon L White が書いた論文「[How to Print Floating-Point Numbers Accurately](https://lists.gnu.org/archive/html/gcl-devel/2012-10/pdfkieTlklRzN.pdf)」を元に作られた dobule 型の実数を ascii に変換するオープンソースなプログラム．

### FreeBSD 版
- [dtoa.c](https://github.com/freebsd/freebsd/blob/master/contrib/gdtoa/dtoa.c)
- [gdtoaimp.h](https://github.com/freebsd/freebsd/blob/master/contrib/gdtoa/gdtoaimp.h)

### Apple 版
- gdtoa
	- [gdtoa-dtoa.c](https://opensource.apple.com/source/Libc/Libc-1353.41.1/gdtoa/FreeBSD/gdtoa-dtoa.c.auto.html)
	- [gdtoaimp.h](https://opensource.apple.com/source/Libc/Libc-1353.41.1/gdtoa/FreeBSD/gdtoaimp.h.auto.html)
- Libc
	- [Libc-1353.11.2](https://opensource.apple.com/source/Libc/Libc-1353.11.2/)
- libsystem
	- [Libsystem-1281](https://opensource.apple.com/release/macos-1015.html)

### 個人で作られた版
- [precise_round(3) -- 丸め桁数指定つきの「正確な」round関数](https://github.com/hnw/precise-round)

### 他言語版
- cpp 版: [Tencent/rapidjson/include/rapidjson/internal/dtoa.h](https://github.com/Tencent/rapidjson/blob/master/include/rapidjson/internal/dtoa.h)
- Go 版: [tanaton/dtoa](https://github.com/tanaton/dtoa)

