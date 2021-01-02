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
	unsigned int address;
} Parameters_t;

extern Parameters_t ParamBlock;

extern void burner(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_BURNER_H_ */
