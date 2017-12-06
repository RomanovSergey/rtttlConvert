

#include <stdio.h>
#include <stdint.h>

int main()
{
	static const char *song = NULL;
	static uint16_t tempo;
	static char duration;
	static char octave;
	uint8_t temp_duration, temp_octave, current_note, dot_flag;
	static uint16_t calc_duration = 0; // in ms

	printf( "rtttl convert \n" );


	duration = 4; // Длительность звучания = 4/4 = 1
	tempo = 63;   // Темп = 63 bpm
	octave = 6;   // Октава = 6-я

	while (*song != ':') song++;  // Найти первый символ ':'
	song++;                       // Пропустить символ ':'
	while ( *song != ':' )            // Повторять до символа ':'
	{
		if (*song == 'd')           // Если это символ длительности ноты, то
		{
			duration = 0;             // временно duration=0
			song++;                   // Перейти к следующему символу
			while (*song == '=') song++;  // Символ '=' пропустить
			while (*song == ' ') song++;  // Пробелы пропустить
			// Если символ – число, то записать его в переменную duration
			if (*song >= '0' && *song <='9') duration = *song - '0';
			song++;                   // Перейти к следующему символу
			// Если символ – тоже число (длительность может состоять из двух цифр), то
			if (*song >= '0' && *song <= '9')
			{ // умножить на 10 и добавить текущую цифру (символ)
				duration = duration*10 + (*song - '0');
				song++;                 // Перейти к следующему символу
			}
			while (*song == ',') song++;  // Пропустить символ ','
		}
		if (*song == 'o')           // Если это указатель октавы, то
		{
			octave = 0;               // временно octave=0
			song++;                   // Перейти к следующему символу
			while (*song == '=') song++;  // Пропустить символ '='
			while (*song == ' ') song++;  // Пропустить пробелы
			// Если символ – число, то записать его в переменную octave
			if (*song >= '0' && *song <= '9') octave = *song - '0';
			song++;                   // Перейти к следующему символу
			while (*song == ',') song++;  // Пропустить символ ','
		}
		if (*song == 'b')           // Если это указатель темпа (нот в минуту),
		{
			tempo = 0;                // временно tempo=0
			song++;                   // Перейти к следующему символу
			while (*song == '=') song++;  // Пропустить символ '='
			while (*song == ' ') song++;  // Пропустить пробелы
			// Теперь читать цифры темпа (может быть число из 3 цифр)
			if (*song >= '0' && *song <= '9') tempo = *song - '0';
			song++;                   // Перейти к следующему символу
			if (*song >= '0' && *song <= '9')
			{
				tempo = tempo*10 + (*song - '0'); // Число состоит из 2 цифр
				song++;                 // Перейти к следующему символу
				if (*song >= '0' && *song <= '9')
				{
					tempo = tempo*10 + (*song - '0'); // Число состоит из 3 цифр
					song++;               // Перейти к следующему символу
				}
			}
			while (*song == ',') song++;  // Пропустить символ ','
		}
		while (*song == ',') song++;    // Пропустить символ ','
	}
	song++;                       // Перейти к следующему символу






	while ( *song ) {

		current_note = 255;         // Текущая нота - пауза
		temp_octave = octave;       // Установить октаву по умолчанию
		temp_duration = duration;   // Установить длительность по умолчанию
		dot_flag = 0;               // Сбросить флаг dot flag
		// Определить и обработать префикс длительности
		if ( *song >= '0' && *song <= '9' ) {
			temp_duration = *song - '0';
			song++;
			if ( *song >= '0' && *song <= '9' ) {
				temp_duration = temp_duration*10 + (*song - '0');
				song++;
			}
		}
		// Определить ноту
		switch (*song)
		{
		case 'c': current_note = 0; break;    // C (до)
		case 'd': current_note = 2; break;    // D (ре)
		case 'e': current_note = 4; break;    // E (ми)
		case 'f': current_note = 5; break;    // F (фа)
		case 'g': current_note = 7; break;    // G (соль)
		case 'a': current_note = 9; break;    // A (ля)
		case 'b': current_note = 11; break;   // B (си)
		case 'p': current_note = 255; break;  // пауза
		}
		song++;                     // Перейти к следующему символу
		// Если за символом ноты следует символ #,
		if (*song=='#'){
			current_note++;   // то увеличить на 1 переменную note
			//(A-A#, C-C#, D-D#, F-F#, G-G#)
			song++;                   // Перейти к следующему символу
		}
		// Если следующий символ '.' (признак увеличения длительности ноты на 50%),
		if (*song=='.'){
			dot_flag = 1;             // то установить флаг dot flag
			song++;                   // Перейти к следующему символу
		}
		// Анализировать признак октавы
		if (*song >= '0' && *song <= '9'){
			temp_octave = *song - '0';// Установить октаву
			song++;                   // Перейти к следующему символу
		}
		if (*song=='.'){// За признаком октавы должна следовать точка
			dot_flag = 1;             // Если это так, то установить флаг dot flag
			song++;                   // Перейти к следующему символу
		}
		while (*song == ',') song++;    // Пропустить символ ','


		// Рассчитать длительность ноты
		calc_duration = ( 60000/tempo ) / ( temp_duration );
		calc_duration *= 4;         // Нота состоит из 4 интервалов темпа
		// Если флаг dot flag установлен, то увеличить длительность на 50%
		if (dot_flag) {
			calc_duration = (calc_duration*3) / 2;
		}
		// Если текущая нота – не пауза, то проигрывать эту ноту с использованием
		// функции sound
		if ( current_note < 255 ) {
			//uint32_t x = (uint32_t)1000000 / note[temp_octave-4][current_note];
			//TIM_SetAutoreload( TIM17, x );
			//TIM_SetCompare1( TIM17, 30 );
		}
		else
		{ // Если текущая нота = 255 (пауза), то реализовать задержку
			//TIM_SetCompare1(TIM17, 0);
		}


		// перейти к следующей мелодии
		//vTaskDelay(calc_duration / portTICK_RATE_MS);
	}

	return 0;
}
