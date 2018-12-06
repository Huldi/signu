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
)

const RESULT_SUCCESS uint = 0x00

func AddrBTC(arr []byte) string{
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_id(x,i)
	}
    address := C.malloc(C.sizeof_char * 64)
    defer C.free(unsafe.Pointer(address))
    C.btc_adr((*C.char)(address),&x)
    var buf []byte
    buf = C.GoBytes(address,64)
    str := string(buf[:x])
    return str
}

func AddrETH(arr []byte) string{
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_id(x,i)
	}
    address := C.malloc(C.sizeof_char * 64)
    defer C.free(unsafe.Pointer(address))
    C.eth_adr((*C.char)(address),&x)
    var buf []byte
    buf = C.GoBytes(address,64)
    str := string(buf[:x])
    return str
}

func StartWallet(){
    C.start_wallet()
}

func WithdrawalOfMoney(data []byte,len_data int, id []byte, sign *[]byte, currency string) bool{
    CreateData(data[:])

    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(id));i++{
        x = (C.uchar)(id[i])
        C.generate_id(x,i)
    }

    signature := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(signature))

    var cryptocurrency int
    if currency == "btc"{
	    cryptocurrency = 0x01
    } else if currency == "eth"{
	    cryptocurrency = 0x02
    }

    C.withdrawal_of_money((*C.uchar)(signature),(C.uint)(len_data),(C.uint)(cryptocurrency))
    *sign = C.GoBytes(signature,64)
    return true
}

func CheckTransaction(data []byte,length int, id []byte, sign []byte, currency string) bool{
    tmp := make([]byte,len(data)+len(sign))
    copy(tmp[:length],data[:])
    copy(tmp[length:],sign)
    CreateData(tmp[:])

    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(id));i++{
        x = (C.uchar)(id[i])
        C.generate_id(x,i)
    }
    var cryptocurrency int
    if currency == "btc"{
	    cryptocurrency = 0x01
    } else if currency == "eth"{
	    cryptocurrency = 0x02
    }

    res := C.check_transaction((C.uint)(len(data)),(C.uint)(cryptocurrency))

    
    if res != 0x00{
        return false
    }else{
        return true
    }
}

func CreateData(arr []byte ){
    var x C.uchar
    var i C.uint
    for i = 0;i < C.uint(len(arr));i++{
        x = (C.uchar)(arr[i])
        C.generate_data(x,i)
	}
}

func EnclaveGenerateKeyPair() uint{
   res := C.generate_key_pair_without_enclave()
   return uint(res)
}

func EnclaveExportPublicKey(buf *[]byte) uint{
    open_key := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(open_key))
    res := C.export_public_key_without_enclave((*C.uchar)(open_key))
    if res == C.int(RESULT_SUCCESS){
        *buf = C.GoBytes(open_key,64)
        return uint(res)
    }else{
        return uint(res)
    }
    
}

func EnclaveSignData(data []byte, length int, sign *[]byte) uint{
    CreateData(data[:])

    signature := C.malloc(C.sizeof_uchar * 64)
    defer C.free(unsafe.Pointer(signature))
    

    res := C.sign_data_without_enclave((*C.uchar)(signature),(C.uint)(length))
    if res == C.int(RESULT_SUCCESS){
        *sign = C.GoBytes(signature,64)
        return uint(res)
    }else{
        return uint(res)
    }
}

func EnclaveVerifySign(data []byte,length int, sign []byte) bool{
    tmp := make([]byte,len(data)+len(sign))
    copy(tmp[:length],data[:])
    copy(tmp[length:],sign)
    CreateData(tmp[:])
    
    res := C.verify_signature_without_enclave((C.uint)(len(data)))
    if res != 0x00{
        return false
    }else{
        return true
    }
}
