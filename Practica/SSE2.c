#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

int main() {
   int i, t = 0;
   char *a, *b, *c;
  __m128i v1, v2;

   if(posix_memalign((void**)&a, 16, 16) != 0)
      return 1;

   if(posix_memalign((void**)&b, 16, 16) != 0)
      return 1;

   if(posix_memalign((void**)&c, 16, 16) != 0)
      return 1;

   for (i = 0; i < 16; i++) {
      a[i] = i + 1;
      b[i] = 1;
   }

   v1 = _mm_load_si128((__m128i*)a);
   v2 = _mm_load_si128((__m128i*)b);
   v1 = _mm_adds_epi8(v1,v2);
  _mm_store_si128((__m128i*)c, v1);

   return 0;
}
