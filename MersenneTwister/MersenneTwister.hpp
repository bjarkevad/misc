/* 
   A C++ program for MT19937, with initialization improved 2002/1/26.
   Original C Coded by Takuji Nishimura and Makoto Matsumoto.
   Converted to a C++ class by Andrew J. Bennieston

   Before using, initialize the state by using init(seed)  
   or initArray(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.
   C++ Class modifications copyright (C) 2008, Andrew J. Bennieston.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)

   Feedback on the C++ modifications to andrew */


class MersenneTwister {
	private:
		const int N;
		const int M;
		const unsigned long MATRIX_A;
		const unsigned long UPPER_MASK;
		const unsigned long LOWER_MASK;
		int mti;
		unsigned long* mt;
	public:
		// Constructors
		MersenneTwister()
			: N(624), M(397), MATRIX_A(0x9908b0dfUL),
			UPPER_MASK(0x80000000UL), LOWER_MASK(0x7fffffffUL),
			mti(N+1)
			{ mt = new unsigned long[N]; };

		// Destructor
		~MersenneTwister() { delete[] mt; };
		
		// Initialisation
		void init(unsigned long s);
		void initArray(unsigned long init_key[], int key_length);

		// Random number generation
		unsigned long genInt32(); // Random integer on [0,0xffffffff]
		long genInt31(); // Random integer on [0,0x7fffffff]
		double genRealClosed(); // Random real on closed range [0,1]
		double genReal(); // Random real on half-open range [0,1) (i.e. not including 1)
		double genRealOpen(); // Random real on open range (0,1) (i.e. not including 0 or 1)
		double genReal53(); // Random 53-bit real on [0,1)
};
