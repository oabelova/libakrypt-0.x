/* Тестовый пример иллюстрирует применение режима простой замены с зацеплением (Кузнечик).
   Внимание! Используются не экспортируемые функции.

   test-bckey04.c
*/
 #include <stdio.h>
 #include <stdlib.h>
 #include <ak_bckey.h>
 #include <ak_tools.h>

/*
   инверитрованные тестовые значения взяты из
   https://github.com/gost-engine/engine/blob/master/test_grasshopper.c
*/

 int main( int argc, char *argv[] )
{
  int result = EXIT_FAILURE;

 /* устанавливаем флаг совместимости с openssl: 0 - нет совместимости, 1 - есть */
  int i, j, oc = 0;
  ak_uint8 buf[128], *ptr;
  struct bckey bkey;

 /* значение секретного ключа согласно ГОСТ Р 34.12-2015 */
  ak_uint8 key[32] = {
    0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
    0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00, 0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88 };

 /* последовательность символов для передачи в командную строку
    8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef */
  ak_uint8 openssl_key[32] = {
    0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff,0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
    0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10,0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef };

 /* открытый текст из ГОСТ Р 34.12-2015, приложение А.1, подлежащий зашифрованию */
  ak_uint8 in[64] = {
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
    0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00,
    0x00, 0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11,
    0x11, 0x00, 0x0a, 0xff, 0xee, 0xcc, 0xbb, 0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22
  };
  ak_uint8 openssl_in[64] = {
    0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x00,0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xEE,0xFF,0x0A,
    0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xEE,0xFF,0x0A,0x00,
    0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xEE,0xFF,0x0A,0x00,0x11
  };

 /* инициализационный вектор для режима гаммирования (счетчика) */
  ak_uint8 ivcbc[32] = {
    0x12, 0x01, 0xf0, 0xe5, 0xd4, 0xc3, 0xb2, 0xa1, 0xf0, 0xce, 0xab, 0x90, 0x78, 0x56, 0x34, 0x12,
    0x19, 0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x90, 0x89, 0x78, 0x67, 0x56, 0x45, 0x34, 0x23
  };

  ak_uint8 openssl_ivcbc[16] = {
      0x12,0x34,0x56,0x78,0x90,0xab,0xce,0xf0,0xa1,0xb2,0xc3,0xd4,0xe5,0xf0,0x01,0x12
  };

 /* зашифрованный блок из ГОСТ Р 34.12-2015 */
  ak_uint8 outcbc[64] = {
    0x27, 0xcc, 0x7d, 0x6d, 0x3d, 0x2e, 0xe5, 0x90, 0x4d, 0xfa, 0x85, 0xa0, 0xd4, 0x72, 0x99, 0x68,
    0xac, 0xa5, 0x5e, 0x8d, 0x44, 0x8e, 0x1e, 0xaf, 0xa6, 0xec, 0x78, 0xb4, 0x61, 0xe6, 0x26, 0x28,
    0xd0, 0x90, 0x9d, 0xf4, 0xb0, 0xe8, 0x40, 0x56, 0xe8, 0x99, 0x19, 0xe9, 0xf1, 0xab, 0x7b, 0xfe,
    0x70, 0x39, 0xb6, 0x60, 0x15, 0x9a, 0x2d, 0x1a, 0x63, 0x5c, 0x89, 0x5a, 0x06, 0x88, 0x76, 0x16
  };
  ak_uint8 openssl_outcbc[64] = {
      0x68,0x99,0x72,0xd4,0xa0,0x85,0xfa,0x4d,0x90,0xe5,0x2e,0x3d,0x6d,0x7d,0xcc,0x27,
      0xab,0xf1,0x70,0xb2,0xb2,0x26,0xc3,0x01,0x0c,0xcf,0xa1,0x36,0xd6,0x59,0xcd,0xaa,
      0xca,0x71,0x92,0x72,0xab,0x1d,0x43,0x8e,0x15,0x50,0x7d,0x52,0x1e,0xcd,0x55,0x22,
      0xe0,0x11,0x08,0xff,0x8d,0x9d,0x3a,0x6d,0x8c,0xa2,0xa5,0x33,0xfa,0x61,0x4e,0x71
  };

  FILE *fp = NULL;

 /* сохраняем данные в файл, чтобы можно было потом проверить с помощью библиотеки openssl */
  fp = fopen( "in.dat", "wb" ); fwrite( openssl_in, 63, 1, fp ); fclose( fp );


 /* теперь можно проверить эквивалентность шифрования выполнив следующую команду,
    параметры которой указаны в настоящем файле выше

    openssl enc -e -grasshopper-cbc -in in.dat -out in.dat.enc -iv <значение iv> -K <значение ключа> */

 /* передаем в программу значение флага совместимости */
  if( argc > 1 ) oc = atoi( argv[1] );
  if( oc != 1 ) oc = 0;

 /* инициализируем библиотеку */
  if( !ak_libakrypt_create( ak_function_log_stderr )) return ak_libakrypt_destroy();

 /* устанавливаем нужный вариант совместимости и пересчитываем внутренние таблицы */
  ak_libakrypt_set_option( "openssl_compability", oc );
  ak_bckey_context_kuznechik_init_gost_tables();
  oc ? printf("openssl_compability is ON\n") : printf("openssl_compability is NO\n");

 /* создаем секретный ключ алгоритма Кузнечик */
  if( ak_bckey_context_create_kuznechik( &bkey ) != ak_error_ok )
    return ak_libakrypt_destroy();

 /* устанавливаем секретный ключ */
  ak_bckey_context_set_key( &bkey, oc ? openssl_key : key, sizeof( key ));

 /* зашифровываем и расшифровываем всего четыре блока данных */

  ak_bckey_context_encrypt_cbc( &bkey, oc ? openssl_in : in, buf, sizeof( in ),
                                                                   oc ? openssl_ivcbc : ivcbc, oc ? sizeof(openssl_ivcbc) : sizeof(ivcbc) );
  fp = fopen( "in.dat.encx", "wb" ); fwrite( buf, 64, 1, fp ); fclose( fp );
  printf("encrypted:\n");
  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 16; j++ ) printf(" %02x", buf[i*16+j] );
    printf("\n");
  }

  ptr = oc ? openssl_outcbc : outcbc;
  printf("\nexpected:\n");
  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 16; j++ ) printf(" %02x", ptr[i*16+j] );
    printf("\n");
  }

  if( ak_ptr_is_equal( buf, ptr, 64 )) {
    printf("Ok\n\n");
    result = EXIT_SUCCESS;
  }  else printf("Wrong\n\n");

  ak_bckey_context_decrypt_cbc( &bkey, oc ? openssl_outcbc : outcbc, buf, sizeof( in ),
                                                                  oc ? openssl_ivcbc : ivcbc, oc ? sizeof(openssl_ivcbc) : sizeof(ivcbc) );
  printf("decrypted:\n");
  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 16; j++ ) printf(" %02x", buf[i*16+j] );
    printf("\n");
  }

  ptr = oc ? openssl_in : in;
  printf("\nexpected:\n");
  for( i = 0; i < 4; i++ ) {
    for( j = 0; j < 16; j++ ) printf(" %02x", ptr[i*16+j] );
    printf("\n");
  }

  if( ak_ptr_is_equal( buf, ptr, 64 )) {
    printf("Ok\n");
    result = EXIT_SUCCESS;
  }  else printf("Wrong\n");

  ak_bckey_context_destroy( &bkey );
  ak_libakrypt_destroy();

 return result;
}
