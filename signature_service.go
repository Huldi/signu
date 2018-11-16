package signature_service

/*
#include "./App/TEE.h"

#cgo LDFLAGS: -L./APP -L. -ltee
*/
// #include <stdio.h>
// #include <stdlib.h>
import "C"

import (
    "unsafe"
    "fmt"
)

const RESULT_SUCCESS uint = 0x00

func BTC_Adr(arr []byte) string{
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_id(x,i)
	}
    victory := C.malloc(C.sizeof_char * 64)
    defer C.free(unsafe.Pointer(victory))
    C.btc_adr((*C.char)(victory),&x)
    var buf []byte
    buf = C.GoBytes(victory,64)
    str := string(buf[:x])
    return str
}

func ETH_Adr(arr []byte) string{
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_id(x,i)
	}
    victory := C.malloc(C.sizeof_char * 64)
    defer C.free(unsafe.Pointer(victory))
    C.eth_adr((*C.char)(victory),&x)
    var buf []byte
    buf = C.GoBytes(victory,64)
    str := string(buf[:x])
    return str
}

func Start_Wallet(){
    C.start_wallet()
    fmt.Println("Да неужели?")
}

func Withdrawal_of_money(data []byte,len_data int, id []byte, sign *[]byte) bool{
    Create_data(data[:])

    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(id));i++{
        x = (C.uchar)(id[i])
        C.generate_id(x,i)
    }

    signature := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(signature))

    C.withdrawal_of_money((*C.uchar)(signature),(C.uint)(len_data))
    *sign = C.GoBytes(signature,64)
    return true
}

func Check_Transaction(data []byte,length int, id []byte, sign []byte) bool{
    tmp := make([]byte,len(data)+len(sign))
    copy(tmp[:length],data[:])
    copy(tmp[length:],sign)
    Create_data(tmp[:])

    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(id));i++{
        x = (C.uchar)(id[i])
        C.generate_id(x,i)
    }

    res := C.check_transaction((C.uint)(len(data)))

    if res != 0x00{
        return false
    }else{
        return true
    }
}

func Create_data(arr []byte ){
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_data(x,i)
	}
}

func Enclave_generate_key_pair() uint{
   //res := C.generate_key_pair_within_enclave()
   res := C.generate_key_pair_without_enclave()
   return uint(res)
}

func Enclave_export_public_key(buf *[]byte) uint{
    open_key := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(open_key))
   // res := C.export_public_key_from_enclave((*C.uchar)(open_key))
    res := C.export_public_key_without_enclave((*C.uchar)(open_key))
    if res == C.int(RESULT_SUCCESS){
        *buf = C.GoBytes(open_key,64)
        return uint(res)
    }else{
        return uint(res)
    }
    
}

func Enclave_sign_data(data []byte, length int, sign *[]byte) uint{
    Create_data(data[:])

    signature := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(signature))
    
    //res := C.sign_data_in_enclave((*C.uchar)(signature),(C.uint)(length))
    res := C.sign_data_without_enclave((*C.uchar)(signature),(C.uint)(length))
    if res == C.int(RESULT_SUCCESS){
        *sign = C.GoBytes(signature,64)
        return uint(res)
    }else{
        return uint(res)
    }
}

func Enclave_verify_sign(data []byte,length int, sign []byte) bool{
    tmp := make([]byte,len(data)+len(sign))
    copy(tmp[:length],data[:])
    copy(tmp[length:],sign)
    Create_data(tmp[:])
   
    //res := C.verify_signature_in_enclave((C.uint)(len(data)))
    res := C.verify_signature_without_enclave((C.uint)(len(data)))
    if res != 0x00{
        return false
    }else{
        return true
    }
}
