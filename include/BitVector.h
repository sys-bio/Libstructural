#ifndef BINARY_VECTOR_H
#define	BINARY_VECTOR_H

#include "GlobalData.h"
#include <stdint.h>

typedef uint32_t u_int32_t; 
typedef uint64_t u_int64_t;


#define MAX_REACTION_COUNT 448
//Lookup table to compute cardinality of bit vectors
extern int gefm_BIT_COUNT_LOOKUP[];
//Computes cardinality of a 32-bit variable
#define CARDINALITY32(n) gefm_BIT_COUNT_LOOKUP[n & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 8) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 16) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 24) & 0xFF]
//Computes cardinality of a 32-bit variable
#define CARDINALITY64(n) gefm_BIT_COUNT_LOOKUP[n & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 8) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 16) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 24) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 32) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 40) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 48) & 0xFF] + gefm_BIT_COUNT_LOOKUP[(n >> 56) & 0xFF]

//These classes represent bit vectors of various lengths.

class BitVector32 {
	public:

	   BitVector32() {
		  data1 = 0;
	   }

	   BitVector32(int bit) {
		  data1 = 1;
		  data1 <<= bit;
	   }

	   BitVector32(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
		  }
	   }

	   BitVector32& operator =(const BitVector32& a) {
		  data1 = a.data1;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector32& a) {
		  data1 &= a.data1;
	   }

	   void bitwiseOr(const BitVector32& a) {
		  data1 |= a.data1;
	   }

	   bool operator ==(const BitVector32& a) {
		  return (data1 == a.data1);
	   }

	   bool operator !=(const BitVector32& a) {
		  return (data1 != a.data1);
	   }

	   const bool operator[] (int bit) const {
		  u_int32_t mask = 1;
		  return (data1 & (mask << bit));
	   }

	   void setBit(int bit, bool value) {
		  u_int32_t mask = 1;
		  mask <<= bit;
		  if (value) {
			 data1 |= mask;
		  } else {
			 data1 &= ~mask;
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
	   }

	   bool isSuperSetOf(BitVector32& a) {
		  return (data1 & a.data1) == a.data1;
	   }

	   bool notAndEqualsNot(const BitVector32& a) {
		  return (~data1 & a.data1);
	   }

	   int getCardinality() {
		  return CARDINALITY32(data1);
	   }

	private:
	   u_int32_t data1;
	};

	class BitVector64 {
	public:

	   BitVector64() {
		  data1 = 0;
	   }

	   BitVector64(int bit) {
		  data1 = 1;
		  data1 <<= bit;
	   }

	   BitVector64(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
		  }
	   }

	   BitVector64& operator =(const BitVector64& a) {
		  data1 = a.data1;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector64& a) {
		  data1 &= a.data1;
	   }

	   void bitwiseOr(const BitVector64& a) {
		  data1 |= a.data1;
	   }

	   bool operator ==(const BitVector64& a) {
		  return (data1 == a.data1);
	   }

	   bool operator !=(const BitVector64& a) {
		  return (data1 != a.data1);
	   }

	   const bool operator[] (int bit) const {
		  u_int64_t mask = 1;
		  return (data1 & (mask << bit));
	   }

	   void setBit(int bit, bool value) {
		  u_int64_t mask = 1;
		  mask <<= bit;
		  if (value) {
			 data1 |= mask;
		  } else {
			 data1 &= ~mask;
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
	   }

	   bool isSuperSetOf(BitVector64& a) {
		  return (data1 & a.data1) == a.data1;
	   }

	   bool notAndEqualsNot(const BitVector64& a) {
		  return (~data1 & a.data1);
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1);
	   }

	private:
	   u_int64_t data1;
	};

	class BitVector96 {
	public:

	   BitVector96() {
		  data1 = 0;
		  data2 = 0;
	   }

	   BitVector96(int bit) {
		  data1 = 0;
		  data2 = 0;
		  if (bit < 64) {
			 data1 = 1;
			 data1 <<= bit;
		  } else {
			 data2 = 1;
			 data2 <<= (bit - 64);
		  }
	   }

	   BitVector96(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
			 data2 = 0;
		  }
	   }

	   BitVector96& operator =(const BitVector96& a) {
		  data1 = a.data1;
		  data2 = a.data2;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector96& a) {
		  data1 &= a.data1;
		  data2 &= a.data2;
	   }

	   void bitwiseOr(const BitVector96& a) {
		  data1 |= a.data1;
		  data2 |= a.data2;
	   }

	   bool operator ==(const BitVector96& a) {
		  return (data1 == a.data1) && (data2 == a.data2);
	   }

	   bool operator !=(const BitVector96& a) {
		  return (data1 != a.data1) || (data2 != a.data2);
	   }

	   const bool operator[] (int bit) const {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 return (data1 & (mask << bit));
		  } else {
			 bit -= 64;
			 u_int32_t mask = 1;
			 return (data2 & (mask << bit));
		  }
	   }

	   void setBit(int bit, bool value) {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 mask <<= bit;
			 if (value) {
				data1 |= mask;
			 } else {
				data1 &= ~mask;
			 }
		  } else {
			 u_int32_t mask = 1;
			 mask <<= (bit - 64);
			 if (value) {
				data2 |= mask;
			 } else {
				data2 &= ~mask;
			 }
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
		  data2 = 0;
		  data2--;
	   }

	   bool isSuperSetOf(BitVector96& a) {
		  return ((data1 & a.data1) == a.data1) && ((data2 & a.data2) == a.data2);
	   }

	   bool notAndEqualsNot(const BitVector96& a) {
		  if ((~data1 & a.data1) > 0) {
			 return true;
		  }
		  if ((~data2 & a.data2) > 0) {
			 return true;
		  }
		  return false;
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1) + CARDINALITY32(data2);
	   }

	private:
	   u_int64_t data1;
	   u_int32_t data2;
	};

	class BitVector128 {
	public:

	   BitVector128() {
		  data1 = 0;
		  data2 = 0;
	   }

	   BitVector128(int bit) {
		  data1 = 0;
		  data2 = 0;
		  if (bit < 64) {
			 data1 = 1;
			 data1 <<= bit;
		  } else {
			 data2 = 1;
			 data2 <<= (bit - 64);
		  }
	   }

	   BitVector128(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
			 data2 = 0;
		  }
	   }

	   BitVector128& operator =(const BitVector128& a) {
		  data1 = a.data1;
		  data2 = a.data2;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector128& a) {
		  data1 &= a.data1;
		  data2 &= a.data2;
	   }

	   void bitwiseOr(const BitVector128& a) {
		  data1 |= a.data1;
		  data2 |= a.data2;
	   }

	   bool operator ==(const BitVector128& a) {
		  return (data1 == a.data1) && (data2 == a.data2);
	   }

	   bool operator !=(const BitVector128& a) {
		  return (data1 != a.data1) || (data2 != a.data2);
	   }

	   const bool operator[] (int bit) const {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 return (data1 & (mask << bit));
		  } else {
			 bit -= 64;
			 u_int64_t mask = 1;
			 return (data2 & (mask << bit));
		  }
	   }

	   void setBit(int bit, bool value) {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 mask <<= bit;
			 if (value) {
				data1 |= mask;
			 } else {
				data1 &= ~mask;
			 }
		  } else {
			 u_int64_t mask = 1;
			 mask <<= (bit - 64);
			 if (value) {
				data2 |= mask;
			 } else {
				data2 &= ~mask;
			 }
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
		  data2 = 0;
		  data2--;
	   }

	   bool isSuperSetOf(BitVector128& a) {
		  return ((data1 & a.data1) == a.data1) && ((data2 & a.data2) == a.data2);
	   }

	   bool notAndEqualsNot(const BitVector128& a) {
		  if ((~data1 & a.data1) > 0) {
			 return true;
		  }
		  if ((~data2 & a.data2) > 0) {
			 return true;
		  }
		  return false;
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1) + CARDINALITY64(data2);
	   }

	private:
	   u_int64_t data1;
	   u_int64_t data2;
	};

	class BitVector160 {
	public:

	   BitVector160() {
		  data1 = 0;
		  data2 = 0;
		  data3 = 0;
	   }

	   BitVector160(int bit) {
		  data1 = 0;
		  data2 = 0;
		  if (bit < 64) {
			 data1 = 1;
			 data1 <<= bit;
		  } else if (bit < 128) {
			 data2 = 1;
			 data2 <<= (bit - 64);
		  } else {
			 data3 = 1;
			 data3 <<= (bit - 128);
		  }
	   }

	   BitVector160(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
			 data2 = 0;
			 data3 = 0;
		  }
	   }

	   BitVector160& operator =(const BitVector160& a) {
		  data1 = a.data1;
		  data2 = a.data2;
		  data3 = a.data3;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector160& a) {
		  data1 &= a.data1;
		  data2 &= a.data2;
		  data3 &= a.data3;
	   }

	   void bitwiseOr(const BitVector160& a) {
		  data1 |= a.data1;
		  data2 |= a.data2;
		  data3 |= a.data3;
	   }

	   bool operator ==(const BitVector160& a) {
		  return (data1 == a.data1) && (data2 == a.data2) && (data3 == a.data3);
	   }

	   bool operator !=(const BitVector160& a) {
		  return (data1 != a.data1) || (data2 != a.data2) || (data3 != a.data3);
	   }

	   const bool operator[] (int bit) const {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 return (data1 & (mask << bit));
		  } else if (bit < 128) {
			 bit -= 64;
			 u_int64_t mask = 1;
			 return (data2 & (mask << bit));
		  } else {
			 bit -= 128;
			 u_int32_t mask = 1;
			 return (data3 & (mask << bit));
		  }
	   }

	   void setBit(int bit, bool value) {
		  if (bit < 64) {
			 u_int64_t mask = 1;
			 mask <<= bit;
			 if (value) {
				data1 |= mask;
			 } else {
				data1 &= ~mask;
			 }
		  } else if (bit < 128) {
			 u_int64_t mask = 1;
			 mask <<= (bit - 64);
			 if (value) {
				data2 |= mask;
			 } else {
				data2 &= ~mask;
			 }
		  } else {
			 u_int32_t mask = 1;
			 mask <<= (bit - 128);
			 if (value) {
				data3 |= mask;
			 } else {
				data3 &= ~mask;
			 }
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
		  data2 = 0;
		  data2--;
		  data3 = 0;
		  data3--;
	   }

	   bool isSuperSetOf(BitVector160& a) {
		  return ((data1 & a.data1) == a.data1) && ((data2 & a.data2) == a.data2) && ((data3 & a.data3) == a.data3);
	   }

	   bool notAndEqualsNot(const BitVector160& a) {
		  if ((~data1 & a.data1) > 0) {
			 return true;
		  }
		  if ((~data2 & a.data2) > 0) {
			 return true;
		  }
		  if ((~data3 & a.data3) > 0) {
			 return true;
		  }
		  return false;
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1) + CARDINALITY64(data2) + CARDINALITY32(data3);
	   }

	private:
	   u_int64_t data1;
	   u_int64_t data2;
	   u_int32_t data3;
	};

	class BitVector192 {
	public:

	   BitVector192() {
		  data1 = 0;
		  data2 = 0;
		  data3 = 0;
	   }

	   BitVector192(int bit) {
		  data1 = 0;
		  data2 = 0;
		  if (bit < 64) {
			 data1 = 1;
			 data1 <<= bit;
		  } else if (bit < 128) {
			 data2 = 1;
			 data2 <<= bit;
		  } else {
			 data3 = 1;
			 data3 <<= (bit - 128);
		  }
	   }

	   BitVector192(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
			 data2 = 0;
			 data3 = 0;
		  }
	   }

	   BitVector192& operator =(const BitVector192& a) {
		  data1 = a.data1;
		  data2 = a.data2;
		  data3 = a.data3;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector192& a) {
		  data1 &= a.data1;
		  data2 &= a.data2;
		  data3 &= a.data3;
	   }

	   void bitwiseOr(const BitVector192& a) {
		  data1 |= a.data1;
		  data2 |= a.data2;
		  data3 |= a.data3;
	   }

	   bool operator ==(const BitVector192& a) {
		  return (data1 == a.data1) && (data2 == a.data2) && (data3 == a.data3);
	   }

	   bool operator !=(const BitVector192& a) {
		  return (data1 != a.data1) || (data2 != a.data2) || (data3 != a.data3);
	   }

	   const bool operator[] (int bit) const {
		  u_int64_t mask = 1;
		  if (bit < 64) {
			 return (data1 & (mask << bit));
		  } else if (bit < 128) {
			 bit -= 64;
			 return (data2 & (mask << bit));
		  } else {
			 bit -= 128;
			 return (data3 & (mask << bit));
		  }
	   }

	   void setBit(int bit, bool value) {
		  u_int64_t mask = 1;
		  if (bit < 64) {
			 mask <<= bit;
			 if (value) {
				data1 |= mask;
			 } else {
				data1 &= ~mask;
			 }
		  } else if (bit < 128) {
			 mask <<= (bit - 64);
			 if (value) {
				data2 |= mask;
			 } else {
				data2 &= ~mask;
			 }
		  } else {
			 mask <<= (bit - 128);
			 if (value) {
				data3 |= mask;
			 } else {
				data3 &= ~mask;
			 }
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
		  data2 = 0;
		  data2--;
		  data3 = 0;
		  data3--;
	   }

	   bool isSuperSetOf(BitVector192& a) {
		  return ((data1 & a.data1) == a.data1) && ((data2 & a.data2) == a.data2) && ((data3 & a.data3) == a.data3);
	   }

	   bool notAndEqualsNot(const BitVector192& a) {
		  if ((~data1 & a.data1) > 0) {
			 return true;
		  }
		  if ((~data2 & a.data2) > 0) {
			 return true;
		  }
		  if ((~data3 & a.data3) > 0) {
			 return true;
		  }
		  return false;
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1) + CARDINALITY64(data2) + CARDINALITY64(data3);
	   }

	private:
	   u_int64_t data1;
	   u_int64_t data2;
	   u_int64_t data3;
	};

	class BitVector448 {
	public:

	   BitVector448() {
		  data1 = 0;
		  data2 = 0;
		  data3 = 0;
		  data4 = 0;
		  data5 = 0;
		  data6 = 0;
		  data7 = 0;
	   }

	   BitVector448(int bit) {
		  data1 = 0;
		  data2 = 0;
		  data3 = 0;
		  data4 = 0;
		  data5 = 0;
		  data6 = 0;
		  data7 = 0;
		  if (bit < 64) {
			 data1 = 1;
			 data1 <<= bit;
		  } else if (bit < 128) {
			 data2 = 1;
			 data2 <<= (bit - 64);
		  } else if (bit < 192) {
			 data3 = 1;
			 data3 <<= (bit - 128);
		  } else if (bit < 256) {
			 data4 = 1;
			 data4 <<= (bit - 192);
		  } else if (bit < 320) {
			 data5 = 1;
			 data5 <<= (bit - 256);
		  } else if (bit < 384) {
			 data6 = 1;
			 data6 <<= (bit - 320);
		  } else {
			 data7 = 1;
			 data7 <<= (bit - 384);
		  }
	   }

	   BitVector448(bool allbits) {
		  if (allbits) {
			 setAllBits();
		  } else {
			 data1 = 0;
			 data2 = 0;
			 data3 = 0;
			 data4 = 0;
			 data5 = 0;
			 data6 = 0;
			 data7 = 0;
		  }
	   }

	   BitVector448& operator =(const BitVector448& a) {
		  data1 = a.data1;
		  data2 = a.data2;
		  data3 = a.data3;
		  data4 = a.data4;
		  data5 = a.data5;
		  data6 = a.data6;
		  data7 = a.data7;
		  return *this;
	   }

	   void bitwiseAnd(const BitVector448& a) {
		  data1 &= a.data1;
		  data2 &= a.data2;
		  data3 &= a.data3;
		  data4 &= a.data4;
		  data5 &= a.data5;
		  data6 &= a.data6;
		  data7 &= a.data7;
	   }

	   void bitwiseOr(const BitVector448& a) {
		  data1 |= a.data1;
		  data2 |= a.data2;
		  data3 |= a.data3;
		  data4 |= a.data4;
		  data5 |= a.data5;
		  data6 |= a.data6;
		  data7 |= a.data7;
	   }

	   bool operator ==(const BitVector448& a) {
		  return (data1 == a.data1) && (data2 == a.data2) && (data3 == a.data3) && (data4 == a.data4) && (data5 == a.data5) && (data6 == a.data6) && (data7 == a.data7);
	   }

	   bool operator !=(const BitVector448& a) {
		  return (data1 != a.data1) || (data2 != a.data2) || (data3 != a.data3) || (data4 != a.data4) || (data5 != a.data5) || (data6 != a.data6) || (data7 != a.data7);
	   }

	   const bool operator[] (int bit) const {
		  u_int64_t mask = 1;
		  if (bit < 64) {
			 return (data1 & (mask << bit));
		  } else if (bit < 128) {
			 bit -= 64;
			 return (data2 & (mask << bit));
		  } else if (bit < 192) {
			 bit -= 128;
			 return (data3 & (mask << bit));
		  } else if (bit < 256) {
			 bit -= 192;
			 return (data4 & (mask << bit));
		  } else if (bit < 320) {
			 bit -= 256;
			 return (data5 & (mask << bit));
		  } else if (bit < 384) {
			 bit -= 320;
			 return (data6 & (mask << bit));
		  } else {
			 bit -= 384;
			 return (data7 & (mask << bit));
		  }
	   }

	   void setBit(int bit, bool value) {
		  u_int64_t mask = 1;
		  if (bit < 64) {
			 mask <<= bit;
			 if (value) {
				data1 |= mask;
			 } else {
				data1 &= ~mask;
			 }
		  } else if (bit < 128) {
			 mask <<= (bit - 64);
			 if (value) {
				data2 |= mask;
			 } else {
				data2 &= ~mask;
			 }
		  } else if (bit < 192) {
			 mask <<= (bit - 128);
			 if (value) {
				data3 |= mask;
			 } else {
				data3 &= ~mask;
			 }
		  } else if (bit < 256) {
			 mask <<= (bit - 192);
			 if (value) {
				data4 |= mask;
			 } else {
				data4 &= ~mask;
			 }
		  } else if (bit < 320) {
			 mask <<= (bit - 256);
			 if (value) {
				data5 |= mask;
			 } else {
				data5 &= ~mask;
			 }
		  } else if (bit < 384) {
			 mask <<= (bit - 320);
			 if (value) {
				data6 |= mask;
			 } else {
				data6 &= ~mask;
			 }
		  } else {
			 mask <<= (bit - 384);
			 if (value) {
				data7 |= mask;
			 } else {
				data7 &= ~mask;
			 }
		  }
	   }

	   void setAllBits() {
		  data1 = 0;
		  data1--;
		  data2 = 0;
		  data2--;
		  data3 = 0;
		  data3--;
		  data4 = 0;
		  data4--;
		  data5 = 0;
		  data5--;
		  data6 = 0;
		  data6--;
		  data7 = 0;
		  data7--;
	   }

	   bool isSuperSetOf(BitVector448& a) {
		  return ((data1 & a.data1) == a.data1) && ((data2 & a.data2) == a.data2) && ((data3 & a.data3) == a.data3) && ((data4 & a.data4) == a.data4) && ((data5 & a.data5) == a.data5) && ((data6 & a.data6) == a.data6) && ((data7 & a.data7) == a.data7);
	   }

	   bool notAndEqualsNot(const BitVector448& a) {
		  if ((~data1 & a.data1) > 0) {
			 return true;
		  }
		  if ((~data2 & a.data2) > 0) {
			 return true;
		  }
		  if ((~data3 & a.data3) > 0) {
			 return true;
		  }
		  if ((~data4 & a.data4) > 0) {
			 return true;
		  }
		  if ((~data5 & a.data5) > 0) {
			 return true;
		  }
		  if ((~data6 & a.data6) > 0) {
			 return true;
		  }
		  if ((~data7 & a.data7) > 0) {
			 return true;
		  }
		  return false;
	   }

	   int getCardinality() {
		  return CARDINALITY64(data1) + CARDINALITY64(data2) + CARDINALITY64(data3) + CARDINALITY64(data4) + CARDINALITY64(data5) + CARDINALITY64(data6) + CARDINALITY64(data7);
	   }

	private:
	   u_int64_t data1;
	   u_int64_t data2;
	   u_int64_t data3;
	   u_int64_t data4;
	   u_int64_t data5;
	   u_int64_t data6;
	   u_int64_t data7;
};



#endif	/* BINARY_VECTOR_H */
