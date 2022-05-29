## MonoHook
本来想看看沙石镇时光的源码，结果源码是加密的，网上搜了一大堆Unity的加密解密说明，最终找到了这个方法，可以导出解密后的Assembly-CSharp.dll


### 使用

使用也很简单，把编译后得到的version.dll放到游戏根目录即可，运行游戏会在根目录生成解密后的Assembly-CSharp.dll

### 原理

通过Hook：mono-2.0-bdwgc.dll的mono_image_open_from_data_with_name方法，让dll解密后导出。


参考文章：http://www.yxfzedu.com/rs_show/446