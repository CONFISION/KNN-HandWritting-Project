from numpy import *
import os
import operator

#图像矩阵转换成向量
def img2vector(filename):
    returnVect = zeros((1,1089))
    fr = open(filename)
    #array0lines = fr.readlines()#把每一行的结果都读取出来
    #linestr = fr.readline()
    #print(len(fr.readline()))
    for i in range(32):#range(32)是0到32
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0, 32*i+j] = int(lineStr[j])
    return returnVect

#kNN算法核心
def classfy0(inX,dataSet,label,k):
    dataSetSize = dataSet.shape[0]
    diffMat = tile(inX, (dataSetSize,1)) - dataSet
    sqdiffMat = diffMat**2
    sqDistances = sqdiffMat.sum(axis=1)
    Distances = sqDistances**0.5
    sortedDistIndicies = Distances.argsort() #对应距离从小到大的索引排序的数组
    classCount = {} #定义classcount字典
    for i in range(k):
        voteLabel = label[sortedDistIndicies[i]] ##取出前k个元素的类别
        #classCount.get{voteLabel,0}返回该元素对应的值，即0
        classCount[voteLabel] = classCount.get(voteLabel, 0) + 1 #字典的get()方法,返回指定键的值
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)#逆序
    return sortedClassCount[0][0]

def HandwritingClassTest():
    labels = []
    trainingFileList = os.listdir('trainingDigits')
    m =len(trainingFileList)
    trainingMat = zeros((m, 1089))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        labels.append(classNumStr)
        trainingMat[i, :] = img2vector('trainingDigits/%s' % fileNameStr)
    testFileList = os.listdir('testDigits')
    numTestFileList = len(testFileList)
    errorCount = 0
    for i in range(numTestFileList):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        testFileMat = img2vector('testDigits/%s' % fileNameStr)
        ClassifierResult = classfy0(testFileMat, trainingMat,labels, 3)
        print('测试分类为%d，实际分类为%d'%(ClassifierResult,classNumStr))
        if (ClassifierResult != labels[i]):
            errorCount += 1
    print('分类器错误率为%f' % (errorCount / float(numTestFileList)))

if __name__ == '__main__':
    HandwritingClassTest()