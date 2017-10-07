character*1 cmdchar
key_input: select case(cmdchar)
	case('0')
		print *, '文件数限于1-9'
	case('1':'9')
		call RetrieveNumFiles(cmdchar)
	case('A','a')
		call AddEntry
	case('D','d')
		call DeletEntry
	case('H','h')
		call Help
    case default
		print *, '键入错误，请键入H查看帮助'
end select key_input
END
