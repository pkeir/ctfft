
! Recursive FFT implementation inspired by a Single Assignment C version.
! Copyright (C) 2012 Paul Keir
! Distributed under the GNU General Public License. See license.txt for details.

module fftmod

  public :: fft, condense
  integer, parameter :: k = 8  ! kind size
  integer, parameter :: n = SZ ! #define SZ macro. Try 8: gfortran -cpp -DSZ=8

  contains

  function condense(i,x) result (y)
    integer, intent(in)                       :: i
    complex(kind=k), dimension(:), intent(in) :: x
    complex(kind=k), dimension(size(x)/i)     :: y
    y = x(1:size(x):i)
  end function condense

  function cat(a,b) result(res)
    complex(kind=k), dimension(:), intent(in)   :: a, b ! Different lengths OK?
    complex(kind=k), dimension(size(a)+size(b)) :: res
    res(1:size(a))                 = a
    res(size(a)+1:size(a)+size(b)) = b
  end function cat

  recursive function fft(v,rofu) result(res)
    complex(kind=k), dimension(:), intent(in) :: v
    complex(kind=k), dimension(:), intent(in) :: rofu ! size(v/2)
    complex(kind=k), dimension(size(v))       :: res
    complex(kind=k), dimension(size(v)/2)     :: left,     right
    complex(kind=k), dimension(size(v)/2)     :: fft_left, fft_right
    complex(kind=k), dimension(size(rofu)/2)  :: rofu_select

    if (size(v) > 2) then
      left        = condense(2,v)
      right       = condense(2,cshift(v,1))

      rofu_select = condense(2, rofu)

      fft_left    = fft(left,  rofu_select) !!
      fft_right   = fft(right, rofu_select)

      fft_right   = fft_right * rofu

      res = cat(fft_left + fft_right, fft_left - fft_right)
    else
      res = [v(1)+v(2),v(1)-v(2)]
    endif

  end function fft

end module fftmod

program p

  use fftmod
  integer :: i
  integer, parameter :: m = n/2
  real(kind=k), parameter :: pi = 3.141592653589793d0
  complex(kind=k), dimension(m) :: d = (/ (exp(-2. * pi * (0, 1.) / n) ** i, i = 0, m-1) /)
  complex(kind=k), dimension(n) :: x,y
  real(kind=8)    :: t1, t2

  x = (/ (i, i=0,n-1) /)
  x = 2 * pi / n * x
  x = sin(x)

!  write(*,'(8 ("(", en12.3, ",", en12.3, ")", /))') x
  print *, x
!  print *
  y = fft(x,d)
  y = 2 * pi / n * y
!  write(*,'(8 ("(", en12.3, ",", en12.3, ")", /))') y
  print *, y

end program p

