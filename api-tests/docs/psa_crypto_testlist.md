# PSA Crypto Testcase checklist 



| **Group**    | **Test** | **Function**  | **Scenario**     | **Return Value** | **Steps**   | **Test Case**    |
|------------------------------|-----------|--------------------------|-----------------------------------------------------------------------------------------------------------------------|-----------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| **Basic**    | test_c001 | psa_crypto_init     | Library initialization | PSA_SUCCESS  | Calling this function should return SUCCESS     |    |
|     | | | Applications must call this function before calling any other function in this module.  | PSA_SUCCESS  | Try calling crypto operations doing a crypto_init should be successful(can be covered as part of other testcase) |    |
|     | | | Applications may call this function more than once. Once a call succeeds, subsequent calls are guaranteed to succeed. | PSA_SUCCESS  | Try calling multiple crypto init and should return SUCCESS     |    |
|     | | | Applications must call this function before calling any other function in this module.  | PSA_ERROR_BAD_STATE    | Try calling crypto operations without doing a crypto_init should return FAILURE    |    |
|     | | |    |    |    |    |
| **Key Management**     | test_c002 | psa_import_key | Import a key in binary format.   | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key  <br /> 7. Export a key in binary format  <br /> 8. Check if original key data matches with the exported data | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_NOT_SUPPORTED     | Calling this function with incorrect key type   | Incorrect key type     |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Key data greater than the algorithm size  <br /> 2. Incorrect key data size  <br /> 3. Invalid key slot  <br /> 4. Zero key slot |
|     | | |    | PSA_ERROR_OCCUPIED_SLOT     | Pass the key slot to store data which is already occupied | Already occupied key slot   |
|     | test_c003 | psa_export_key | Export a key in binary format    | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key  <br /> 7. Export a key in binary format  <br /> 8. Check if original key data matches with the exported data | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_BUFFER_TOO_SMALL  | Calling this function with buffer size less than required | Less buffer size  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Zero key slot   <br /> 2. Invalid key slot |
|     | | |    | PSA_ERROR_BAD_STATE    | Calling this function with key policy that cannot be exported  | Invalid key policy usage    |
|     | | |    | PSA_ERROR_EMPTY_SLOT   | Calling this function with empty key slot  | Empty key slot    |
|     | test_c004 | psa_export_public_key    | Export a public key or the public part of a key pair in binary   format. | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key  <br /> 7. Export a key in binary format  <br /> 8. Check if original key data matches with the exported data | 1. 2048 RSA public key  <br /> 2. 2048 RSA keypair  <br /> 3. EC Public key  <br /> 4. EC keypair     |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key  <br /> 7. Export a key in binary format  <br /> 8. Check if original key data matches with the exported data | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. DES 64 bit key  <br /> 5. Triple DES 2-Key  <br /> 6. Triple DES 3-Key |
|     | | |    | PSA_ERROR_BUFFER_TOO_SMALL  | Calling this function with buffer size less than required | Less buffer size  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Zero key slot   <br /> 2. Invalid key slot |
|     | | |    | PSA_ERROR_BAD_STATE    | Calling this function with key policy that cannot be exported  | Invalid key policy usage    |
|     | test_c005 | psa_destroy_key     | Destroy a key and restore the slot to its default state.  | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key  <br /> 7. Destroy a key and restore the slot to its default state  <br /> 8. Check that if the key metadata are destroyed   | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Invalid key slot  <br /> 2. Zero key slot  <br /> 3. Empty key slot    |
|     | test_c006 | psa_get_key_information  | Get basic metadata about a key.  | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get basic metadata about a key     | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Zero key slot   <br /> 2. Invalid key slot |
|     | | |    | PSA_ERROR_EMPTY_SLOT   | Pass the key slot number which has the key type as none   |  Empty key slot   |
|     | NO TEST   | psa_key_policy_set_usage | Set the   standard fields of a policy structure.     | void    | Void function. Covered as part of other cases   |    |
|     | | |    |    |    |    |
| **Key Policies**  | test_c007 | psa_set_key_policy  | Set the usage policy on a key slot.   | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4.  Set the usage policy on a key slot  <br /> 5. Import the key data into the key slot  <br /> 6. Get the usage policy for a key slot  <br /> 7. Check if the policy matches the original input     | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Invalid key policy  <br /> 2. Zero key slot  <br /> 3. Invalid key slot     |
|     | | |    | PSA_ERROR_OCCUPIED_SLOT     | Pass the key slot to store data which is already occupied | Already occupied key slot   |
|     | test_c008 | psa_get_key_policy  | Get the usage policy for a key slot   | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4. Set the usage policy on a key slot  <br /> 5. Change the lifetime of a key slot  <br /> 6. Import the key data into the key slot  <br /> 7. Get the usage policy for a key slot  <br /> 8. Retrieve the usage field of a policy structure  <br /> 9. Retrieve the algorithm field of a policy structure  <br /> 10. Make sure they match the original value | 1. 16 Byte AES  <br /> 2. 24 Byte AES  <br /> 3. 32 Byte AES  <br /> 4. 2048 RSA public key  <br /> 5. 2048 RSA keypair  <br /> 6. DES 64 bit key  <br /> 7. Triple DES 2-Key  <br /> 8. Triple DES 3-Key  <br /> 9. EC Public key  <br /> 10. EC keypair |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Zero key slot   <br /> 2. Invalid key slot |
|     | test_c009 | psa_set_key_lifetime     | Change the lifetime of a key slot.    | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4. Set the usage policy on a key slot  <br /> 5. Change the lifetime of a key slot  <br /> 6. Import the key data into the key slot  <br /> 7. Reset the system  <br /> 8. After reboot check if the contents of persistent and write once key data are preserved   | 1. Volatile keys  <br /> 2. Persistent keys  <br /> 3. Write once keys    |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with incorrect argument should return failure | 1. Zero key slot  <br /> 2. Invalid key slot  <br /> 3. Invalid key lifetime   |
|     | | |    | PSA_ERROR_OCCUPIED_SLOT     | If implementation does not supports changing the lifetime of preoccupied slot, calling this function with already occupied slot should return failure(Impdef) | Pre occupied key slot  |
|     | test_c010 | psa_get_key_lifetime     | Retrieve   the lifetime of a key slot.     | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Initialize a key policy structure to a default that forbids all usage of the key  <br /> 3. Set the standard fields of a policy structure  <br /> 4. Set the usage policy on a key slot  <br /> 5. Change the lifetime of a key slot  <br /> 6. Import the key data into the key slot  <br /> 7. Get the lifetime of a key slot    | Testing only volatile keys as other key types are currently not supported     |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with invalid parameter should return this error    | 1. Invalid key policy  <br /> 2. Zero key slot  <br /> 3. Invalid key slot  <br /> 4. Empty key slot  |
|     | | |    |    |    |    |
| **Message Authentication Codes** | test_c011 | psa_hash_start | Start a multipart hash operation.     | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Start a multipart hash operation | 1. MD2  <br /> 2. MD4  <br /> 3. MD5  <br /> 4. RIPEMD160  <br /> 5. SHA1  <br /> 6. SHA224  <br /> 7. SHA256  <br /> 8. SHA512  <br /> 9. SHA512_224  <br /> 10. SHA512_256  <br /> 11. SHA3_224 1 <br /> 2. SHA3_256 1 <br /> 3. SHA3_384 1 <br /> 4. SHA3_512 |
|     | | |    | PSA_ERROR_NOT_SUPPORTED     | Calling this function with unsupported algorithm should return error     | Invalid hash algorithm |
|     | test_c012 | psa_hash_update     | Add a message fragment to a multipart hash operation.     | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Start a multipart hash operation  <br /> 3. Add a message fragment to a multipart hash operation     | 1. MD2  <br /> 2. MD4  <br /> 3. MD5  <br /> 4. RIPEMD160  <br /> 5. SHA1  <br /> 6. SHA224  <br /> 7. SHA256  <br /> 8. SHA384  <br /> 9. SHA512  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function without calling the psa_hash_start() should return error     | Inactive operation handle   |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with completed operation handle should return error     | Completed operation handle  |
|     | test_c013 | psa_hash_verify     | Finish the calculation of the hash of a message and compare it with an expected value.  | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Start a multipart hash operation  <br /> 3. Add a message fragment to a multipart hash operation  <br /> 4. Finish the calculation of the hash of a message and compare it with an expected value | 1. MD2  <br /> 2. MD4  <br /> 3. MD5  <br /> 4. RIPEMD160  <br /> 5. SHA1  <br /> 6. SHA224  <br /> 7. SHA256  <br /> 8. SHA384  <br /> 9. SHA512  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with inactive operation handle should return error | Inactive operation handle   |
|     | | |    | PSA_ERROR_INVALID_SIGNATURE | Calling this function with incorrect expected value should return error  | 1. Incorrect expected hash value  <br /> 2. Incorrect expected hash length  |
|     | test_c014 | psa_hash_finish     | Finish the calculation of the hash of a message.     | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Start a multipart hash operation  <br /> 3. Add a message fragment to a multipart hash operation  <br /> 4. Finish the calculation of the hash of a message  <br /> 5. Compare it with the expected value | 1. MD2  <br /> 2. MD4  <br /> 3. MD5  <br /> 4. RIPEMD160  <br /> 5. SHA1  <br /> 6. SHA224  <br /> 7. SHA256  <br /> 8. SHA384  <br /> 9. SHA512  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling this function with an inactive operation handle should return error   | Inactive operation handle   |
|     | | |    | PSA_ERROR_BUFFER_TOO_SMALL  | Calling this function with a hash buffer whose size is less than the algorithm output should return error   | Buffer size less than required   |
|     | test_c015 | psa_hash_abort | Abort a hash operation.     | PSA_SUCCESS  | 1. Initialize the PSA crypto library  <br /> 2. Start a multipart hash operation  <br /> 3. Abort a hash operation     | 1. MD2  <br /> 2. MD4  <br /> 3. MD5  <br /> 4. RIPEMD160  <br /> 5. SHA1  <br /> 6. SHA224  <br /> 7. SHA256  <br /> 8. SHA384  <br /> 9. SHA512  |
|     | | |    | PSA_ERROR_INVALID_ARGUMENT  | Calling psa_hash_finish after calling psa_hash_abort should return error |    |

# License
Arm PSA test suite is distributed under Apache v2.0 License.

--------------

*Copyright (c) 2018, Arm Limited and Contributors. All rights reserved.*