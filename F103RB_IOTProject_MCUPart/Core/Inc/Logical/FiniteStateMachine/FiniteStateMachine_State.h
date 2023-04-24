#ifndef INC_USER_FSM_STATE_LIST_H_
#define INC_USER_FSM_STATE_LIST_H_

typedef enum
{
	// include state here,NUM_STATE is used to count the number of state
	START_STATE,
	CHAR_0_STATE,
	DELIMIT_0_STATE,
	CHAR_TYPE_STATE,
	DELIMIT_1_STATE,
	CHAR_VALUE_STATE,
	END_STATE,

	NUM_STATE,
} FiniteStateMachine_State;

#endif /* INC_USER_FSM_STATE_LIST_H_ */
