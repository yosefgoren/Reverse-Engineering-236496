# 1: Dry
## A:


```
void minus(unsigned* a, unsigned* b, unsigned len){
    unsigned carry = 0;
    for(unsigned i = 0; i < len; ++i){
        long unsigned int m = b[i]+carry;
        if(a[i] < m){
            carry = 1;
            a[i] = ((long unsigned int)0xffffffff-m) + a[i]
        }
    }
}
```