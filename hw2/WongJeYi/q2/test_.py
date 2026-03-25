import _vector
import math
import pytest

def test_zero():
    result= _vector.radian([0,0],[1,2])
    assert math.isnan(result) 

def test_90deg():
    result= _vector.radian([1,0],[0,1])
    assert result == pytest.approx(math.pi / 2)

def test_45deg():
    result= _vector.radian([0.5,0.5],[0,1])
    assert result == pytest.approx(math.pi / 4)

def test_0deg():
    result= _vector.radian([0,5],[0,1])
    assert result==0

def test_270deg():
    result= _vector.radian([1,0],[0,-1])
    assert result ==pytest.approx(((math.pi)/ 2)+math.pi)