#modeled after Riya Kumar's Code from geeksforgeeks.org 'Implementing Checksum using Python'


#checksum of sent message

def findchecksum(message, k):

    c1 = message[0:k]
    c2 = message[k:2*k]
    c3 = message[2*k:3*k]
    c4 = message[3*k:4*k]#message is split into sizes of k

    sum = bin(int(c1, 2) + int(c2, 2) + int(c3, 2) + int(c4, 2))[2:]
    #packets are converted to ints and added together, and then turned into binary

    if(len(sum) > k):#if the length of the sum exceeds k
        x = len(sum) - k #x is the excess bits
        sum = bin(int(sum[0:x], 2) + int(sum[x:], 2))[2:] #add the excess bits to the remaining bits

    if (len(sum) < k): #if the length of the sum is less than k
        sum = '0' * (k - len(sum)) + sum #pad with leading zeros

    checksum = ''
    for i in sum:
        if ( i == '1'):
            checksum += '0'
        else:
            checksum += '1'
    return checksum

    #calculates the complement of sum

def findreceivedchecksum(message, k, checksum):
    c1 = message[0:k]
    c2 = message[k:2*k]
    c3 = message[2*k:3*k]
    c4 = message[3*k:4*k]

    receiversum = bin(int(c1, 2) + int(c2, 2) + int(checksum, 2) + int(c3, 2) + int(c4, 2) + int(checksum, 2))[2:]

    if (len(receiversum) > k):
        x = len(receiversum) - k
        receiversum = bin(int(receiversum[0:x], 2) + int(receiversum[x:], 2))[2:]

    if (len(receiversum) < k):
        receiversum = '0' * (k - len(receiversum)) + receiversum

    receiverchecksum = ''
    for i in receiversum:
        if (i == '1'):
            receiverchecksum += '0'
        else:
            receiverchecksum += '1'

    return receiverchecksum


message = "10101000101011101010101001111010"

k = 8

checksum = findchecksum(message, k)

receiverchecksum = findreceivedchecksum(message, k, checksum)


print("Sender Side Checksum: ", checksum)

print("Receiver Side Checksum: ", receiverchecksum)

finalsum = bin(int(checksum, 2) + int(receiverchecksum, 2))[2:]

finalcomp = ''
for i in finalsum:
    if(i == '0'):
        finalcomp += '0'
    else:
        finalcomp += '1'

#when added together, the checksum should cancel out
#if they do, there are no errors
if (int(finalcomp, 2) == 0):
    print("NO ERRORS")
else:
    print("ERRORS DETECTED")
