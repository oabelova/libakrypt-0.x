/* ----------------------------------------------------------------------------------- */
/* Пример example-24.c                                                             */
/* ----------------------------------------------------------------------------------- */
#include <stdio.h>
#include <libakrypt.h>
#include <libakrypt-base.h>

int main( void )
{
    if( ak_libakrypt_create( NULL ) != ak_true ) {
        /* инициализация выполнена не успешно, следовательно, выходим из программы */
        ak_libakrypt_destroy();
        return EXIT_FAILURE;
    }
    
    char data[128] = "test data for hash";
  
    struct hash ctx; /* контекст функции хеширования */
  
    int error = ak_error_ok;

    /* буффер длиной 32 байта (256 бит) для получения результата */
    ak_uint8 out[32];

    /* инициализируем контекст функции хешиирования */
    if(( error = ak_hash_create_streebog256( &ctx )) != ak_error_ok ) {
        ak_error_message( error, __func__ , "wrong initialization of streenbog256 context" );
    }

    /* берем хеш от строки data */
    ak_hash_ptr( &ctx, data, sizeof(data), out, sizeof( out ));
    
    if(( error = ak_error_get_value()) != ak_error_ok ) {
        ak_error_message( error, __func__ , "invalid calculation of streebog256 code" );
    }
    
    //выводим
    for (int i=0;i<sizeof(out);i++){
        printf("%c", out[i]);
    }
    
    printf("\n");
    
    ak_hash_destroy(&ctx);
    ak_libakrypt_destroy();
    return EXIT_SUCCESS;
    
} 
  
