/*
 The MIT License (MIT)
Copyright (c) 2013 <AthrunArthur>
Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/
#ifndef FF_AUTO_INC_CONST_H_
#define FF_AUTO_INC_CONST_H_
namespace ff{
  namespace detail{
    template <size_t N>
    struct TN{char m[N];};
    template<>
    struct TN<0>{};

    template <class T> char tfn(T);
     
    template <class T, size_t N, size_t L>
    struct inc_v{
            const static size_t v = sizeof (tfn(TN<N-1>())) == 1 ? inc_v<T, N-1, L>::v : sizeof (tfn(TN<N-1>())) ;
    };
    template<class T, size_t L>
    struct inc_v<T, 0, L>{
            const static size_t v = 1;
    };
  }//end namespace detail
}//end namespace ff
#define fc_read(tag) (ff::detail::inc_v<tag, 250, __LINE__>::v )
#define fc_inc(tag) \
      namespace ff{ namespace detail { TN<fc_read(tag) + 1> tfn(TN<fc_read(tag)>); } }

#define fc_add(tag, no) \
      namespace ff{ namespace detail { TN<fc_read(tag) + no> tfn(TN<fc_read(tag)>) ;} }

#endif
