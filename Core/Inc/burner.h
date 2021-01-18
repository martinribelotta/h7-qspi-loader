/*
 * burner.h
 *
 *  Created on: 1 ene. 2021
 *      Author: martin
 */

#ifndef INC_BURNER_H_
#define INC_BURNER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void *MemBuffer;
	unsigned int MemBufferSize;
	void (*ErasePage)(unsigned int addr);
	void (*FlashCopy)(unsigned int flashaddr, unsigned int size);
	void (*FlashVerify)(unsigned int flashaddr, unsigned int size);
} FlashAlgorithm_t;

extern const FlashAlgorithm_t FlashAlgo;

extern void burner(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_BURNER_H_ */
