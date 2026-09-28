/*!
 * \file StringLog.h
 * \brief Класс записи данных в журнал.
 *
 */
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED


//#include "stddef.h"
#include <stdint.h>

extern uint32_t saveLog;


extern uint32_t __end_of_log_section;

#define ENABLE_LOGGING

#ifdef ENABLE_LOGGING

//! \brief Формат вывода чисел в журнал.
enum NUMBER_TYPE : uint8_t {
	hex,	//!< Вывод чисел в шестандцатеричном формате.
	dec,	//!< Вывод чисел в десятичном формате.
	dbl		//!< Вывод чисел в формате с плавающей точкой (не реализовано).
};

//! \brief Результат выполнения теста.
enum TEST_RESULT : bool {
    TRUE,   //!< Отрицательный результат.
    FALSE   //!< Положительный результат.
};

//! \brief Класс записи данных в журнал.
class StringLog {
	static const uint8_t ACCURACY = 12;			//!< Точность, с которой выводится число с плавающей точкой.
	static const uint8_t SYMBOL_FOR_NUM = '\t';	//!< Разделитель при записи в журнал.
    static NUMBER_TYPE number_format;			//!< Формат вывода чисел.

	int FindTheLength(uint32_t high);

	void operator += (const char *);
	void operator /= (double);
	void operator += (int32_t);
	void operator *= (uint32_t);

public:
	//! \brief Конструктор класса StringLog.
	StringLog() = default;

	int ConvIntToHexChar(unsigned long long numeral, uint8_t *buffer);
	int ConvToInt(unsigned long long numeral, uint8_t *buffer);

    static constexpr const char* endl               = "\r\n";               //!< Перевод строки.
    static constexpr const char* test_started       = "Запуск  ";           //!< Запись в журнал при запуске теста.
    static constexpr const char* test_finished      = "Выполнен";           //!< Запись в журнал при выполнении теста.
    static constexpr const char* result_success     = "Норма";              //!< Запись в журнал положительной диагностики.
    static constexpr const char* result_fault       = "Отказ";              //!< Запись в журнал отрицательной диагностики.
    static constexpr const char* result_exception   = "Возникло исключение";//!< Запись в журнал возникновение исключения.

	static constexpr const char* str_received			= "получено";				//!< Запись в журнал полученного результата.
	static constexpr const char* str_expected			= "ожидаемое";              //!< Запись в журнал ожидаемого результата.
	static constexpr const char* str_timeout_occurred 	= "Превышено вр.ожидания";  //!< Запись в журнал превышения времени выполнения.
	static constexpr const char* str_not_equal 			= "≠";                      //!< Запись в журнал знака неравенства.
	static constexpr const char* str_minus 				= "-";                      //!< Запись в журнал знака неравенства.

	friend StringLog& operator<<(StringLog& os, const char* text);
	friend StringLog& operator<<(StringLog& os, int32_t intValue);
	friend StringLog& operator<<(StringLog& os, NUMBER_TYPE num_type);
	friend StringLog& operator<<(StringLog& os, TEST_RESULT node);

	//! \brief Возвращает начальный адреса журнала.
	//! \return uint32_t - адрес начала логирования.
	static uint32_t get_start_zone() {
		return reinterpret_cast<uint32_t>(startLogZone);
	}

	static void SetDefaultAddress();
//********************************************************************************************************************************************************************
private:
	static uint32_t memoryLastAddress;					//!< Конечный адрес журнала.
	static const uint32_t loggingStopSize = 100;		//!< Ограничитель по размеру журнала.
	static uint8_t *logAddress;							//!< Текущий адрес журнала.
	static uint8_t *startLogZone;						//!< Начальный адрес журнала.
	static uint16_t reset;								//!< Признак сброса указателя адреса в начальное значение.


public:
	static bool IsEndLogAddress();
	static uint32_t GetDumpLength();
	static void SetLogAddrInNewLocation(uint8_t *oldAddrLocation, uint8_t *newAddrLocation);
//********************************************************************************************************************************************************************
};
#endif	// ENABLE_LOGGING

#endif // STRING_H_INCLUDED
