/*!
 * \file StringLog.cpp
 * \brief Класс записи данных в журнал.
 */

#include "StringLog.h"

#ifdef ENABLE_LOGGING

NUMBER_TYPE StringLog::number_format = NUMBER_TYPE::dec;

uint32_t StringLog::memoryLastAddress = (uint32_t)(&__end_of_log_section);

/*!
 * \brief Оператор записи в журнал текста.
 * \param s - текст.
 * \return void.
 */
void StringLog::operator += (const char *s) {
#ifndef MODEL_TESTING_ENABLED

	if(StringLog::IsEndLogAddress())
		return;

	*StringLog::logAddress++ = SYMBOL_FOR_NUM;

	for(int i = 0; s[i] != '\0'; i++, StringLog::logAddress++) {
		*StringLog::logAddress = s[i];
	}

#endif //MODEL_TESTING_ENABLED
}

/*!
 * \brief Оператор записи в журнал числа в десятичном формате.
 * \param numeral - выводимое число.
 * \return void.
 */
void StringLog::operator += (int32_t numeral) {
#ifndef MODEL_TESTING_ENABLED
	if(StringLog::IsEndLogAddress())
			return;

    *StringLog::logAddress++ = SYMBOL_FOR_NUM;

	if(numeral < 0)
	{
		numeral *= -1;
		*StringLog::logAddress++ = '-';
	}

	StringLog::logAddress += ConvToInt(numeral, StringLog::logAddress);

#endif //MODEL_TESTING_ENABLED
}

/*!
 * \brief Оператор записи в журнал числа в шестнадцатеричном формате.
 * \param numeral - выводимое число.
 * \return void.
 */
void StringLog::operator *= (uint32_t numeral) {
#ifndef MODEL_TESTING_ENABLED
	if(StringLog::IsEndLogAddress())
		return;

    *StringLog::logAddress++ = SYMBOL_FOR_NUM;
    StringLog::logAddress += ConvIntToHexChar(numeral, StringLog::logAddress);

#endif //MODEL_TESTING_ENABLED
}

/*!
 * \brief Оператор записи в журнал числа с плавающей точкой.
 * \param numeral - число.
 * \return void.
 */
void StringLog::operator /= (double numeral) {
#ifndef MODEL_TESTING_ENABLED

	if(StringLog::IsEndLogAddress())
		return;

    *StringLog::logAddress++ = SYMBOL_FOR_NUM;

	if(numeral < 0)
	{
		numeral *= -1;
		*StringLog::logAddress++ = '-';
	}

	uint32_t hi = (uint32_t)numeral;
	StringLog::logAddress += ConvToInt(hi, StringLog::logAddress);
	*StringLog::logAddress++ = '.';

	uint32_t lo = (uint32_t)((numeral - hi) * 1000000);
	int length = ConvToInt(lo, StringLog::logAddress);
	StringLog::logAddress += length;

	length = ACCURACY - length;

	while (length--)
		*StringLog::logAddress++ = '0';

#endif //MODEL_TESTING_ENABLED
}

//******************************************************************************
/*!
 * \brief Оператор записи в журнал текста.
 * \param os - ссылка на объект журнала.
 * \param text - текст.
 * \return StringLog& os - ссылка на объект журнала.
 */
StringLog& operator<<(StringLog& os, const char* text) {
#ifndef MODEL_TESTING_ENABLED
	if(StringLog::IsEndLogAddress())
		return os;

	*StringLog::logAddress++ = StringLog::SYMBOL_FOR_NUM;

	for(int i = 0; text[i] != '\0'; i++, StringLog::logAddress++) {
		*StringLog::logAddress = text[i];
	}
#endif //MODEL_TESTING_ENABLED
	return os;
}

/*!
 * \brief Оператор записи в журнал целочисленных значений.
 * \param os - ссылка на объект журнала.
 * \param intValue - записываемое в журнал число.
 * \return StringLog& os - ссылка на объект журнала.
 */
StringLog& operator<<(StringLog& os, int32_t intValue) {
#ifndef MODEL_TESTING_ENABLED
	if(StringLog::IsEndLogAddress())
		return os;

	*StringLog::logAddress++ = StringLog::SYMBOL_FOR_NUM;

	switch(os.number_format) {
		case hex:
			StringLog::logAddress += os.ConvIntToHexChar(intValue, StringLog::logAddress);
			break;
		case dec:
		default:

			if(intValue < 0)
			{
				intValue *= -1;
				*StringLog::logAddress++ = '-';
			}
			StringLog::logAddress += os.ConvToInt(intValue, StringLog::logAddress);
			break;
	}
#endif //MODEL_TESTING_ENABLED
	return os;
}

/*!
 * \brief Оператор настройки формата вывода чисел.
 * \param os - ссылка на объект журнала.
 * \param num_type - формат вывода чисел.
 * \return StringLog& os - ссылка на объект журнала.
 */
StringLog& operator<<(StringLog& os, NUMBER_TYPE num_type) {
#ifndef MODEL_TESTING_ENABLED
	os.number_format = num_type;
#endif //MODEL_TESTING_ENABLED
	return os;
}


StringLog& operator<<(StringLog& os, TEST_RESULT result) {
#ifndef MODEL_TESTING_ENABLED
    os << ((result) ? StringLog::result_fault : StringLog::result_success);
#endif //MODEL_TESTING_ENABLED
    return os;
}


//********************************************************************************************************************************************************************
uint8_t* StringLog::logAddress = reinterpret_cast<uint8_t*>(&saveLog);
uint8_t* StringLog::startLogZone = reinterpret_cast<uint8_t*>(&saveLog);
uint16_t StringLog::reset = 0;

/*!
 * \brief Узнать дошли ли до конца журнала.
 * \return bool - Признак конца журнала - true, иначе false
 */
bool StringLog::IsEndLogAddress() {
	return (memoryLastAddress - (uint32_t)logAddress) < loggingStopSize;
}

/*!
 * \brief Получить размер журнала.
 *
 * Рассчитывается текущий объем журнала.
 *
 * \return uint32_t - Текущий объем журнала.
 */
uint32_t StringLog::GetDumpLength() {
	return ((uint32_t*)logAddress - (uint32_t*)&saveLog) * 4;
}

/*!
 * \brief Установить новый адрес журнала с учётом размера старого журнала.
 *
 * Рассчитывается текущий размер журнала и выставляется новый адрес журнала с
 * учетом старого размера.
 *
 * \param oldAddrLocation - старый адрес журнала
 * \param newAddrLocation - новый адрес журнала
 * \return void.
 */
void StringLog::SetLogAddrInNewLocation(uint8_t *oldAddrLocation, uint8_t *newAddrLocation) {
	logAddress = newAddrLocation + ((uint32_t)logAddress - (uint32_t)oldAddrLocation);
}
//********************************************************************************************************************************************************************
/*!
 * \brief Конвертирование десятичного числа в строку шестнадцатеричного представления
 *
 * Производится считывние регистров, с дальнейшим сравнением с эталонными значениями
 *
 * \param numeral - входное значение
 * \param buffer - буфер для записи полученной строки
 * \return int -Длина полученной строки.
 */
int StringLog::ConvIntToHexChar(unsigned long long numeral, uint8_t* buffer) {

    unsigned long long high = numeral;
    int lengthHigh = 0;
	do {
		lengthHigh++;
	} while (high /= 16);


	int index = lengthHigh;

	static const char hexVals[] = {"0123456789ABCDEF"};

	*buffer++ = '0';
	*buffer++ = 'x';
	lengthHigh += 2;

    do {
        buffer[--index] = hexVals[numeral % 16];
    } while(numeral /= 16);

	return lengthHigh;
}

/*!
 * \brief Вспомогательная функция. Нахождение длины числа.
 *
 * Определяется количество символов в числе.
 *
 * \param high - входное значение.
 * \return int - Длина числа.
 */
int StringLog::FindTheLength(uint32_t high) {
	int lengthHigh = 0;

	do {
		lengthHigh++;
	} while (high /= 10);

	return lengthHigh;
}

/*!
 * \brief Конвертирование десятичного числа в строку десятичного представления
 *
 * Производится считывние регистров, с дальнейшим сравнением с эталонными значениями
 *
 * \param numeral - входное значение
 * \param buffer - буфер для записи полученной строки
 * \return int - Длина полученной строки.
 */
int StringLog::ConvToInt(unsigned long long numeral, uint8_t *buffer) {
    int index = 0, len = 0;
	len = index = FindTheLength(numeral);
    do {
        buffer[--index] = (numeral % 10) + '0';
    } while(numeral /= 10);

	return len;
}


/*!
 * \brief Установка указателя текущего положения журнала в исходное значение.
 * \return void.
 */
void StringLog::SetDefaultAddress() {
	reset++;
#ifdef SET_DEFAULT_LOG_ADDR
	logAddress = startLogZone;
#endif //SET_DEFAULT_LOG_ADDR
}
//********************************************************************************************************************************************************************

#endif	// ENABLE_LOGGING
