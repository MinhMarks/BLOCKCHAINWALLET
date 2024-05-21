# from tinyec import registry
# import secrets

# curve = registry.get_curve('secp256r1')

# privKey = secrets.randbelow(curve.field.n)
# pubKey = privKey * curve.g
# print("private key:", privKey)
# print("public key:", pubKey)

from cryptography.hazmat.primitives import serialization as crypto_serialization
from cryptography.hazmat.primitives.asymmetric import ec,utils
from cryptography.hazmat.backends import default_backend as crypto_default_backend
from cryptography.hazmat.primitives import hashes
import binascii
import sys

curve=0
message="Hello 123"

if (len(sys.argv)>1):
	message=sys.argv[1]	
if (len(sys.argv)>2):
	curve=int(sys.argv[2])



key=ec.generate_private_key(ec.SECP256K1())
if curve==1: key=ec.generate_private_key(ec.SECP256R1())
elif curve==2: key=ec.generate_private_key(ec.SECP384R1())
elif curve==3: key=ec.generate_private_key(ec.SECP521R1())
elif curve==4: key=ec.generate_private_key(ec.SECP224R1())
elif curve==5: key=ec.generate_private_key(ec.SECP192R1())
with open('output.txt', 'r') as f:
	try:
		
		private_key = key.private_bytes(crypto_serialization.Encoding.PEM,crypto_serialization.PrivateFormat.PKCS8,crypto_serialization.NoEncryption())
		print(f"\nPrivate key:\n{private_key.decode()}", file=f)

		public_key = key.public_key().public_bytes(crypto_serialization.Encoding.PEM,crypto_serialization.PublicFormat.SubjectPublicKeyInfo)


		print(f"Public key:\n{public_key.decode()}", file=f)

		signature = key.sign(message.encode(),ec.ECDSA(hashes.SHA256()))

		sig_bytes=binascii.b2a_hex(signature).decode()

		b=len(sig_bytes)//2
		print(f"Signature (DER): {sig_bytes}", file=f)
		print(f"r={utils.decode_dss_signature(signature)[0]}\ns={utils.decode_dss_signature(signature)[1]}", file=f)


		rtn=key.public_key().verify(signature, message.encode(),ec.ECDSA(hashes.SHA256()))
		if (rtn==None): print("Signature verified", file=f)
		else: print("Signature failed", file=f)

		print("\n== Key details ==", file=f)

		public_key = key.public_key().public_numbers()
		print(f"Public key: Name: {public_key.curve.name}\nKey size: {public_key.curve.key_size} ", file=f)
		print(f"\nPrivate key: {key.private_numbers().private_value}", file=f)

		print(f"Public key: ({public_key.x}, {public_key.y})", file=f)

	except Exception as e:
		print("Public key error: ",e)