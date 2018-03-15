package cn.chao.BlogFileProcessor;

import java.io.IOException;

public interface ISynchronizer {
    void synchronizeFileAndDatabase() throws IOException;
}
