/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/LCD_DRIVER.h"
#include "MCAL/GPIO_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LCD_TOTAL_PINS_NUM NUMBER_OF_DATA_LINES + 3
#define LCD_USER_STATE_BUSY 1
#define LCD_USER_STATE_READY 0
#define LCD_REQ_TYPE_WRITE 0
#define LCD_REQ_TYPE_CLEAR 1
#define LCD_REQ_TYPE_SET_P 2
#define LCD_MAX_BUFFER_SIZE 5
#define STATIC_STATE_BUSY 0
#define STATIC_STATE_READY 1
#define COMMAND_NIBBLE_HIGH 1
#define COMMAND_NIBBLE_LOW 0
#define WRITE_COMMAND_STATE 0
#define WRITE_DATA_STATE 1

#define LCD_8_PIN_COMMAND_FUNC_SET 0x30
#define LCD_8_PIN_COMMAND_ONOFF 0x08
#define LCD_8_PIN_COMMAND_CLEAR 0x01
#define LCD_8_PIN_COMMAND_ENTRY 0x04
#define LCD_4_PIN_COMMAND_1_ST_FUNC_SET 0x02
#define LCD_4_PIN_COMMAND_2_ND_FUNC_SET 0x20
#define LCD_4_PIN_COMMAND_ONOFF 0x00
#define LCD_4_PIN_COMMAND_CLEAR 0x01
#define LCD_4_PIN_COMMAND_ENTRY 0x00
#define LCD_COMMAND_GOTOXY 128
#define LCD_SECOND_LINE_OFFSET 0x40

#define LCD_1_BIT_OFFSET 1
#define LCD_2_BIT_OFFSET 2
#define LCD_3_BIT_OFFSET 3
#define LCD_4_BIT_OFFSET 4

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    const u8_t *U_string;
    u8_t U_length;
    u8_t U_State;
    u8_t U_Type;
    u8_t X_Position;
    u8_t Y_Position;
    u8_t Curr_Pos;
    CallBack_t CallBack;
} User_Req_t;

/*
typedef struct
{
    u8_t Curr_Pos;
    CallBack_t CallBack;
} Write_Req_t;

typedef struct
{
    CallBack_t CallBack;
} Clear_Req_t;

typedef struct
{
    u8_t X_Position;
    u8_t Y_Position;
    CallBack_t CallBack;
} SetP_Req_t;
*/
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t LCDs_PinCfg;
static u8_t LCD_State = LCD_STATE_OFF;
static u8_t LCD_DigitCounter = 0;
static User_Req_t User_Resquest[LCD_MAX_BUFFER_SIZE];
static u8_t User_CurrentRequest = 0;
// static Write_Req_t Write_Request;
// static Clear_Req_t Clear_Request;
// static SetP_Req_t SetP_Request;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void LCD_WriteToPins(u8_t Info, u8_t State);
static void Init_State_Func();
static void OperationState_WriteFunc();
static void OperationState_ClearFunc();
static void OperationState_SetPFunc();

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status LCD_Init()
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t LcdPins[LCD_TOTAL_PINS_NUM];
    u8_t Index;

    for (Index = 0; Index < NUMBER_OF_DATA_LINES; Index++)
    {
        LcdPins[Index].Pin = LCDs_PinCfg.LCD_data_pins[Index].Pin;
        LcdPins[Index].Port = LCDs_PinCfg.LCD_data_pins[Index].Port;
        LcdPins[Index].Mode = GPIO_MODE_OP_PP;
        LcdPins[Index].Speed = GPIO_SPEED_MED;
    }
    LcdPins[NUMBER_OF_DATA_LINES].Pin = LCDs_PinCfg.E_pin.Pin;
    LcdPins[NUMBER_OF_DATA_LINES].Port = LCDs_PinCfg.E_pin.Port;
    LcdPins[NUMBER_OF_DATA_LINES].Mode = GPIO_MODE_OP_PP;
    LcdPins[NUMBER_OF_DATA_LINES].Speed = GPIO_SPEED_MED;

    LcdPins[NUMBER_OF_DATA_LINES + 1].Pin = LCDs_PinCfg.R_S_pin.Pin;
    LcdPins[NUMBER_OF_DATA_LINES + 1].Port = LCDs_PinCfg.R_S_pin.Port;
    LcdPins[NUMBER_OF_DATA_LINES + 1].Mode = GPIO_MODE_OP_PP;
    LcdPins[NUMBER_OF_DATA_LINES + 1].Speed = GPIO_SPEED_MED;

    LcdPins[NUMBER_OF_DATA_LINES + 2].Pin = LCDs_PinCfg.R_W_pin.Pin;
    LcdPins[NUMBER_OF_DATA_LINES + 2].Port = LCDs_PinCfg.R_W_pin.Port;
    LcdPins[NUMBER_OF_DATA_LINES + 2].Mode = GPIO_MODE_OP_PP;
    LcdPins[NUMBER_OF_DATA_LINES + 2].Speed = GPIO_SPEED_MED;

    LOC_Status = GPIO_Init(LcdPins, LCD_TOTAL_PINS_NUM);

    LCD_State = LCD_STATE_INIT;
    return LOC_Status;
}

Error_Status LCD_WriteStringAsync(u8_t *string, u8_t length, CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            User_Resquest[Index].U_string = string;
            User_Resquest[Index].U_length = length;
            User_Resquest[Index].Curr_Pos = 0;
            User_Resquest[Index].CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_WRITE;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;

            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status LCD_SetCursorPositionAsync(u8_t X_pos, u8_t Y_pos, CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            User_Resquest[Index].X_Position = X_pos;
            User_Resquest[Index].Y_Position = Y_pos;
            User_Resquest[Index].CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_SET_P;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

Error_Status LCD_ClearScreenAsync(CallBack_t CB)
{
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            User_Resquest[Index].CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_CLEAR;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = Status_LCD_Full_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
    }

    return LOC_Status;
}

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
static void Init_State_Func()
{
    static u8_t Init_Call_Count = 0;

    u8_t LOC_FunctionSetCommand = LCD_8_PIN_COMMAND_FUNC_SET;
    u8_t LOC_DisplayOnOffCommand = LCD_8_PIN_COMMAND_ONOFF;
    u8_t LOC_DisplayClearCommand = LCD_8_PIN_COMMAND_CLEAR;
    u8_t LOC_EntryModeCommand = LCD_8_PIN_COMMAND_ENTRY;

    /*set the font and number of lines configuration to the command*/
    LOC_FunctionSetCommand |= (FONT_SIZE << LCD_2_BIT_OFFSET);
    LOC_FunctionSetCommand |= (NUMBER_OF_LINES << LCD_3_BIT_OFFSET);

    /*set the blink, cursor and display state configuration to the command*/
    LOC_DisplayOnOffCommand |= (BLINK_STATE);
    LOC_DisplayOnOffCommand |= (CUSOR_STATE << LCD_1_BIT_OFFSET);
    LOC_DisplayOnOffCommand |= (DISPLAY_STATE << LCD_2_BIT_OFFSET);

    /*set the font and number of lines configuration to the command*/
    LOC_EntryModeCommand |= (SHIFT_STATE);
    LOC_EntryModeCommand |= (INCREMENT_STATE << LCD_1_BIT_OFFSET);

    if (Init_Call_Count < 2)
    {
        /*call the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 4)
    {
        /*call the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 6)
    {
        /*call the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 8)
    {
        /*call the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Call_Count = 0;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS
static void Init_State_Func()
{
    static u8_t Init_Call_Count = 0;

    u8_t LOC_FunctionSetCommand = LCD_4_PIN_COMMAND_1_ST_FUNC_SET;
    u8_t LOC_DisplayOnOffCommand = LCD_4_PIN_COMMAND_ONOFF;
    u8_t LOC_DisplayClearCommand = LCD_4_PIN_COMMAND_CLEAR;
    u8_t LOC_EntryModeCommand = LCD_4_PIN_COMMAND_ENTRY;

    /********************************** call the function set command **********************************/
    if (Init_Call_Count < 4)
    {
        /*first call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 8)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_FunctionSetCommand = LCD_4_PIN_COMMAND_2_ND_FUNC_SET;
        LOC_FunctionSetCommand |= (FONT_SIZE << LCD_2_BIT_OFFSET);
        LOC_FunctionSetCommand |= (NUMBER_OF_LINES << LCD_3_BIT_OFFSET);
        /*second call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the display on/off command **********************************/
    else if (Init_Call_Count < 12)
    {
        /*set the blink, cursor and display state configuration to the command*/
        LOC_DisplayOnOffCommand |= (BLINK_STATE);
        LOC_DisplayOnOffCommand |= (CUSOR_STATE << LCD_1_BIT_OFFSET);
        LOC_DisplayOnOffCommand |= (DISPLAY_STATE << LCD_2_BIT_OFFSET);

        /*setting the DB7 bit to high*/
        LOC_DisplayOnOffCommand |= (1 << LCD_3_BIT_OFFSET);

        /*call of the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the display clear command **********************************/
    else if (Init_Call_Count < 16)
    {
        /*call of the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the entry mode set command **********************************/
    else if (Init_Call_Count < 20)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_EntryModeCommand |= (SHIFT_STATE);
        LOC_EntryModeCommand |= (INCREMENT_STATE << LCD_1_BIT_OFFSET);

        /*call of the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Call_Count = 0;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
static void LCD_WriteToPins(u8_t Info, u8_t State)
{
    static u8_t Command_State = STATIC_STATE_READY;
    u8_t Index;

    switch (Command_State)
    {
    case STATIC_STATE_READY:
        GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
        GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
        for (Index = 0; Index < 8; Index++)
        {
            GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                              LCDs_PinCfg.LCD_data_pins[Index].Pin,
                              ((Info >> Index) & 1));
        }
        GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
        Command_State = STATIC_STATE_BUSY;
        break;

    case STATIC_STATE_BUSY:
        GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
        Command_State = STATIC_STATE_READY;

        break;

    default:
        break;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS
static void LCD_WriteToPins(u8_t Info, u8_t State)
{
    static u8_t Command_State = STATIC_STATE_READY;
    static u8_t Command_Nibble = COMMAND_NIBBLE_HIGH;
    u8_t Index;

    switch (Command_Nibble)
    {
    case COMMAND_NIBBLE_HIGH:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
            GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                                  LCDs_PinCfg.LCD_data_pins[Index].Pin,
                                  ((Info >> (Index + 4)) & 1));
            }
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_LOW;
            break;

        default:
            break;
        }
        break;

    case COMMAND_NIBBLE_LOW:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_Set_PinValue(LCDs_PinCfg.R_W_pin.Port, LCDs_PinCfg.R_W_pin.Pin, GPIO_STATE_RESET);
            GPIO_Set_PinValue(LCDs_PinCfg.R_S_pin.Port, LCDs_PinCfg.R_S_pin.Pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_Set_PinValue(LCDs_PinCfg.LCD_data_pins[Index].Port,
                                  LCDs_PinCfg.LCD_data_pins[Index].Pin,
                                  ((Info >> Index) & 1));
            }
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_SET);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_Set_PinValue(LCDs_PinCfg.E_pin.Port, LCDs_PinCfg.E_pin.Pin, GPIO_STATE_RESET);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_HIGH;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
static void OperationState_WriteFunc()
{
    static u8_t write_state = STATIC_STATE_READY;

    if (User_Resquest[User_CurrentRequest].Curr_Pos < User_Resquest[User_CurrentRequest].U_length)
    {
        switch (write_state)
        {
        case STATIC_STATE_READY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + User_Resquest[User_CurrentRequest].Curr_Pos), WRITE_DATA_STATE);
            write_state = STATIC_STATE_BUSY;
            break;
        case STATIC_STATE_BUSY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + User_Resquest[User_CurrentRequest].Curr_Pos), WRITE_DATA_STATE);
            write_state = STATIC_STATE_READY;
            User_Resquest[User_CurrentRequest].Curr_Pos++;
            break;

        default:
            break;
        }
    }
    else
    {
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;
        User_Resquest[User_CurrentRequest].Curr_Pos = 0;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        //   LCD_DigitCounter++;
        //
        //   /*check if the current position is the end of the first line*/
        //   if (LCD_DigitCounter == NUM_OF_DIGITS)
        //   {
        //       /*go to the beginning of the second line*/
        //       LCD_SetCursorPositionAsync(1, 0, NULL);
        //   }
        //   /*check if the current position is the end of the second line*/
        //   else if (LCD_DigitCounter == NUM_OF_DIGITS * 2)
        //   {
        //       /*go to the beginning of the first line*/
        //       LCD_SetCursorPositionAsync(0, 0, NULL);
        //   }
        //   else
        //   {
        //       /*do nothing*/
        //   }

        if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
        {
            User_Resquest[User_CurrentRequest - 1].CallBack();
        }
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS
static void OperationState_WriteFunc()
{
    static u8_t write_state = STATIC_STATE_READY;
    static u8_t CommandState = 0;
    if (User_Resquest[User_CurrentRequest].Curr_Pos < User_Resquest[User_CurrentRequest].U_length)
    {
        switch (write_state)
        {
        case STATIC_STATE_READY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + User_Resquest[User_CurrentRequest].Curr_Pos), WRITE_DATA_STATE);
            if (CommandState)
            {
                write_state = STATIC_STATE_BUSY;
                CommandState = 0;
            }
            else
            {
                CommandState++;
            }
            break;
        case STATIC_STATE_BUSY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + User_Resquest[User_CurrentRequest].Curr_Pos), WRITE_DATA_STATE);
            if (CommandState)
            {
                write_state = STATIC_STATE_READY;
                User_Resquest[User_CurrentRequest].Curr_Pos++;
                CommandState = 0;
            }
            else
            {
                CommandState++;
            }
            break;

        default:
            break;
        }
    }
    else
    {
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;
        User_Resquest[User_CurrentRequest].Curr_Pos = 0;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        //    LCD_DigitCounter++;
        //
        //    /*check if the current position is the end of the first line*/
        //    if (LCD_DigitCounter == NUM_OF_DIGITS)
        //    {
        //        /*go to the beginning of the second line*/
        //        LCD_SetCursorPositionAsync(1, 0, NULL);
        //    }
        //    /*check if the current position is the end of the second line*/
        //    else if (LCD_DigitCounter == NUM_OF_DIGITS * 2)
        //    {
        //        /*go to the beginning of the first line*/
        //        LCD_SetCursorPositionAsync(0, 0, NULL);
        //    }
        //    else
        //    {
        //        /*do nothing*/
        //    }

        if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
        {
            // GPIO_Set_PinValue(GPIO_PORT_A,GPIO_PIN_11,GPIO_STATE_SET);
            User_Resquest[User_CurrentRequest - 1].CallBack();
        }
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
static void OperationState_ClearFunc()
{
    static u8_t Clear_state = STATIC_STATE_READY;
    u8_t LOC_DisplayClearCommand = LCD_8_PIN_COMMAND_CLEAR;

    switch (Clear_state)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        LCD_SetCursorPositionAsync(0, 0, NULL);

        if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
        {
            User_Resquest[User_CurrentRequest - 1].CallBack();
        }
        break;

    default:
        break;
    }
}
#endif
#if NUMBER_OF_DATA_LINES == DATA_4_PINS
static void OperationState_ClearFunc()
{
    static u8_t Clear_state = STATIC_STATE_READY;
    u8_t LOC_DisplayClearCommand = LCD_8_PIN_COMMAND_CLEAR;
    static u8_t CommandState = 0;
    switch (Clear_state)
    {
    case STATIC_STATE_READY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
            Clear_state = STATIC_STATE_BUSY;
            CommandState = 0;
        }
        else
        {
            CommandState++;
        }
        break;
    case STATIC_STATE_BUSY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
            Clear_state = STATIC_STATE_READY;
            User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

            User_CurrentRequest++;
            if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
            {
                User_CurrentRequest = 0;
            }
            CommandState = 0;

            LCD_SetCursorPositionAsync(0, 0, NULL);

            if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
            {
                User_Resquest[User_CurrentRequest - 1].CallBack();
            }
        }
        else
        {
            CommandState++;
        }
        break;

    default:
        break;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_8_PINS
static void OperationState_SetPFunc()
{
    static u8_t SetPState = STATIC_STATE_READY;
    u8_t LOC_Location = LCD_COMMAND_GOTOXY;

    /*check if the location is in the first line*/
    if (User_Resquest[User_CurrentRequest].X_Position == 0)
    {
        LOC_Location += User_Resquest[User_CurrentRequest].Y_Position;
    }
    else
    {
        LOC_Location += (User_Resquest[User_CurrentRequest].Y_Position + LCD_SECOND_LINE_OFFSET);
    }

    switch (SetPState)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        SetPState = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        User_Resquest[User_CurrentRequest].X_Position = 0;
        User_Resquest[User_CurrentRequest].Y_Position = 0;
        SetPState = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
        {
            User_Resquest[User_CurrentRequest - 1].CallBack();
        }
        break;

    default:
        break;
        //  LCD_DigitCounter = (User_Resquest[User_CurrentRequest].X_Position * 16) + User_Resquest[User_CurrentRequest].Y_Position;
    }
}
#endif

#if NUMBER_OF_DATA_LINES == DATA_4_PINS
static void OperationState_SetPFunc()
{
    static u8_t SetPState = STATIC_STATE_READY;
    u8_t LOC_Location = LCD_COMMAND_GOTOXY;
    static u8_t CommandState = 0;

    /*check if the location is in the first line*/
    if (User_Resquest[User_CurrentRequest].X_Position == 0)
    {
        LOC_Location += User_Resquest[User_CurrentRequest].Y_Position;
    }
    else
    {
        LOC_Location += (User_Resquest[User_CurrentRequest].Y_Position + LCD_SECOND_LINE_OFFSET);
    }

    switch (SetPState)
    {
    case STATIC_STATE_READY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
            SetPState = STATIC_STATE_BUSY;
            CommandState = 0;
        }
        else
        {
            CommandState++;
        }
        break;
    case STATIC_STATE_BUSY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
            User_Resquest[User_CurrentRequest].X_Position = 0;
            User_Resquest[User_CurrentRequest].Y_Position = 0;
            SetPState = STATIC_STATE_READY;
            User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

            User_CurrentRequest++;
            if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
            {
                User_CurrentRequest = 0;
            }
            CommandState = 0;

            if (User_CurrentRequest && User_Resquest[User_CurrentRequest - 1].CallBack)
            {
                User_Resquest[User_CurrentRequest - 1].CallBack();
            }
        }
        else
        {
            CommandState++;
        }
        break;

    default:
        break;
    }

    // LCD_DigitCounter = (SetP_Request.X_Position * 16) + SetP_Request.Y_Position;
}
#endif

Error_Status LCD_GetStatus(u8_t *Status)
{
    Error_Status LOC_Status = Status_NOK;

    if (Status == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        *Status = LCD_State;
    }
    return LOC_Status;
}

void LCD_Runnable(void)
{
    switch (LCD_State)
    {
    case LCD_STATE_INIT:
        Init_State_Func();
        break;
    case LCD_STATE_OPER:
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_BUSY)
        {
            switch (User_Resquest[User_CurrentRequest].U_Type)
            {
            case LCD_REQ_TYPE_WRITE:
                OperationState_WriteFunc();
                break;
            case LCD_REQ_TYPE_CLEAR:
                OperationState_ClearFunc();
                break;
            case LCD_REQ_TYPE_SET_P:
                OperationState_SetPFunc();
                break;

            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}
