


// SCRATCH : TODO PROPPER TEST




    qDebug() << "Before" << m_test.nRows() << "x" << m_test.nColumns();
    m_test.resize(3,4);
    qDebug() << "After" << m_test.nRows() << "x" << m_test.nColumns();

    qDebug() << "before" << m_test(1,1);
    m_test.set(1,1, .1234);
    qDebug() << "after" << m_test(1,1);
    m_test.resize(6,3);
    qDebug() << "after resize" << m_test(1,1);
    m_test.set(0,0, 1.1);
    m_test.set(1,1, 2.2);
    m_test.set(4, 2, 3.3);
    m_test.print();
    qDebug() << m_test.values();
    std::vector<float> ins = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6};
    std::vector<float> outs(3);
    m_test.mult(&ins, &outs);
    qDebug() << outs;
